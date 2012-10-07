#include <QProcess>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "downloadtablewidget.h"
#include "../network/download.h"
#include "../network/systemdownload.h"
#include "savestreamdialog.h"
#include "addon.h"

#include "qdebug.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QFileDialog>
#include <QDesktopServices>
#include <QTimer>
#include <QVBoxLayout>
#include <QStringListModel>
#include <QSortFilterProxyModel>
#include <QRegExp>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qnam = new PirateNetworkAccessManager(this);
    downloadStack = new DownloadListModel(this, qnam);
    userSettings = QHash<QString,QVariant>();

    ui->setupUi(this);

    QString clip = QApplication::clipboard()->text();
    if (clip.startsWith("http"))
        ui->editAdress->setText(clip);

    connect(ui->buttonFetch, SIGNAL(clicked()), this, SLOT(fetch()));
    connect(ui->editAdress, SIGNAL(returnPressed()), this, SLOT(fetch()));
    connect(ui->adressTabWidget, SIGNAL(currentChanged(int)), this, SLOT(adressTabChanged(int)));
    connect(ui->tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabChanged(int)));
    connect(ui->webView, SIGNAL(linkClicked(QUrl)), this, SLOT(linkClicked(QUrl)));
    connect(ui->buttonResetSettings, SIGNAL(clicked()), this, SLOT(resetUserSettings()));

    DownloadTableWidget *downloadTable = new DownloadTableWidget(this);
    downloadTable->setModel(downloadStack);
    ui->groupDownloads->layout()->addWidget(downloadTable);

    ui->webView->hide();
    ui->webView->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    ui->webView->settings()->globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    ui->webView->settings()->setUserStyleSheetUrl(QUrl("data:text/css;charset=utf-8;base64,LnYwXzN7ZGlzcGxheTogbm9uZTt9")); // ".v0_3{display: none;}"
    ui->webView->load(QUrl("http://pirateplay.se/playbrowser/browser.html"));

    ui->groupMessages->setLayout(new QVBoxLayout());

    readUserSettings();
    setupTwitter();

    QNetworkReply *addonReply = qnam->get(QNetworkRequest(QUrl("http://pirateplay.se:8080/static/pirateplayer_addons/list")));
    connect(addonReply, SIGNAL(finished()), SLOT(setupAddons()));

    resize(settings.value("MainWindow/size", QSize(900, 580)).toSize());
    move(settings.value("MainWindow/pos", QPoint(200, 200)).toPoint());
    setWindowIcon(QIcon(":/data/16x16/pirateplayer.png"));
}

MainWindow::~MainWindow()
{
    if (ui->tabWidget->currentWidget() == ui->settingsTab)
        writeUserSettings();

    settings.setValue("MainWindow/size", size());
    settings.setValue("MainWindow/pos", pos());

    delete ui;
}

void MainWindow::getStreams(QString url, QVariantMap metaData) {
    ui->tabWidget->setCurrentIndex(0);
    QUrl urlGetStreams = QUrl("http://pirateplay.se/api/get_streams.xml");
    urlGetStreams.addQueryItem("url", url);
    QNetworkRequest req(urlGetStreams);
    QNetworkReply *getStreamsReply = qnam->get(req);
    StreamTableModel *streamList = new StreamTableModel(getStreamsReply, this);
    streamList->setMetaData(metaData);

    connect(streamList, SIGNAL(finished()), SLOT(streamsFound()));
    connect(streamList, SIGNAL(noStreamsFound()), SLOT(noStreamsFound()));

    MessageLabel *msg = new MessageLabel(url, this);
    ((QVBoxLayout*)ui->groupMessages->layout())->insertWidget(0, msg);
    msg->setState(MessageLabel::Fetching);
    messages[streamList] = msg;
}

void MainWindow::streamsFound() {
    StreamTableModel *streams = (StreamTableModel*)QObject::sender();
    MessageLabel *msg = messages[streams];
    QString streamTitle = msg->getContext();

    if (versionNumberGreater(streams->data(streams->index(0, StreamTableModel::VersionColumn), Qt::DisplayRole).toString(),
                             QCoreApplication::instance()->applicationVersion())) {
        msg->setState(MessageLabel::IncompatibleStreams);
        return;
    }

    delete msg;
    messages.remove(streams);

    SaveStreamDialog *streamDialog = new SaveStreamDialog(streams, userSettings, streamTitle, this);
    if (streamDialog->exec() == QDialog::Accepted) {
        downloadStack->addDownload(streamDialog->getUrl(), streamDialog->getFileName());
        if(streamDialog->downloadSubs())
            downloadStack->addDownload(streamDialog->getSubUrl(), streamDialog->getSubFileName());
    }
}

void MainWindow::noStreamsFound() {
    StreamTableModel *streams = (StreamTableModel*)QObject::sender();
    MessageLabel *msg = messages[streams];

    msg->setState(MessageLabel::NoStreamsFound);
    msg->deleteLaterThanLater();
}

void MainWindow::adressTabChanged(int index) {
    if (index == 0)
        ui->webView->hide();
    else
        ui->webView->show();
}

void MainWindow::fetch() {
    getStreams(ui->editAdress->text());
}

void MainWindow::linkClicked(QUrl url) {
    getStreams(url.toString());
}

void MainWindow::enableTwitterButtons(int currentIndex) {
    ui->buttonNextTweet->setEnabled(currentIndex + 1 < twitterWidgetMapper->model()->rowCount());
    ui->buttonPrevTweet->setEnabled(currentIndex > 0);
}

void MainWindow::setupTwitter() {
    TwitterFeedModel *twitterFeed = new TwitterFeedModel(qnam, this);
    twitterFeed->setScreenName("pirateplay_se");

    QSortFilterProxyModel *filterModel = new QSortFilterProxyModel(this);
    filterModel->setSourceModel(twitterFeed);
    filterModel->setFilterRole(Qt::EditRole);
    filterModel->setDynamicSortFilter(true);
    filterModel->setFilterRegExp(QRegExp("^[^@]"));
    filterModel->setFilterKeyColumn(0);

    ui->tweetText->setOpenExternalLinks(true);

    twitterWidgetMapper = new QDataWidgetMapper(this);
    twitterWidgetMapper->setModel(filterModel);
    twitterWidgetMapper->addMapping(ui->tweetText, 0, "text");
    twitterWidgetMapper->toFirst();
    connect(filterModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), twitterWidgetMapper, SLOT(toFirst()));
    enableTwitterButtons(0);
    connect(twitterWidgetMapper, SIGNAL(currentIndexChanged(int)), this, SLOT(enableTwitterButtons(int)));
    connect(ui->buttonPrevTweet, SIGNAL(clicked()), twitterWidgetMapper, SLOT(toPrevious()));
    connect(ui->buttonNextTweet, SIGNAL(clicked()), twitterWidgetMapper, SLOT(toNext()));
}

void MainWindow::tabChanged(int index) {
    if (ui->tabWidget->indexOf(ui->settingsTab) != index)
        writeUserSettings();
}

void MainWindow::readUserSettings() {
#ifndef UBUNTU_12_04
    userSettings["player_cmd"] = settings.value("Location/player_cmd", "ffplay \"%0\"");
#else
    userSettings["player_cmd"] = settings.value("Location/player_cmd", "avplay \"%0\"");
#endif
    userSettings["start_dir"] = settings.value("Location/start_dir", QDir::toNativeSeparators(QDesktopServices::storageLocation(QDesktopServices::HomeLocation)));
    userSettings["filename_template"] = settings.value("Location/filename_template", "%name_-_%title");

    ui->editPlayer->setText(userSettings["player_cmd"].toString());
    ui->editStartDir->setText(userSettings["start_dir"].toString());
    ui->editFilenameTemplate->setText(userSettings["filename_template"].toString());
}

void MainWindow::writeUserSettings() {
    settings.setValue("Location/player_cmd", ui->editPlayer->text());
    settings.setValue("Location/start_dir", ui->editStartDir->text());
    settings.setValue("Location/filename_template", ui->editFilenameTemplate->text());
    userSettings["player_cmd"] = settings.value("Location/player_cmd");
    userSettings["start_dir"] = settings.value("Location/start_dir");
    userSettings["filename_template"] = settings.value("Location/filename_template");
}

void MainWindow::resetUserSettings() {
    settings.remove("Location/player_cmd");
    settings.remove("Location/start_dir");
    settings.remove("Location/filename_template");
    readUserSettings();
}

bool MainWindow::versionNumberGreater(QString leftHand, QString rightHand) {
    QStringList leftHandList = leftHand.split('.');
    QStringList rightHandList = rightHand.split('.');

    for (int i=0; i<leftHandList.count(); i++) {
        if (i == rightHandList.count() ||
            leftHandList.at(i).toInt() > rightHandList.at(i).toInt())
            return true;
        else if (leftHandList.at(i).toInt() < rightHandList.at(i).toInt())
            return false;
    }

    return false;
}

void MainWindow::setupAddons() {
    QNetworkReply *reply = (QNetworkReply*)QObject::sender();
    QStringList addons = ((QString)reply->readAll()).split("\n", QString::KeepEmptyParts);
    for (int i = 0; i < addons.count(); i++) {
        Addon *a = new Addon(addons.at(i), this->qnam, &this->settings, this, ui->tabWidget);
        connect(a, SIGNAL(titleSet(QString)), SLOT(setAddonTitle(QString)));
        ui->tabWidget->insertTab(1, a, "");
    }
}

void MainWindow::setAddonTitle(QString newTitle) {
    ui->tabWidget->setTabText(ui->tabWidget->indexOf((QWidget*)QObject::sender()), newTitle);
}
