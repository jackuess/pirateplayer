#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "downloadtablewidget.h"
#include "../network/download.h"
#include "../network/systemdownload.h"
#include "savestreamdialog.h"

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
    ui->webView->load(QUrl("http://pirateplay.se:8080/playbrowser/browser.html"));

    ui->groupMessages->setLayout(new QVBoxLayout());

    setupTwitter();
    readUserSettings();

    resize(settings.value("MainWindow/size", QSize(900, 580)).toSize());
    move(settings.value("MainWindow/pos", QPoint(200, 200)).toPoint());
    setWindowIcon(QIcon(":/data/16x16/pirateplayer.png"));
}

MainWindow::~MainWindow()
{
    if (ui->tabWidget->currentIndex() == 1)
        writeUserSettings();

    settings.setValue("MainWindow/size", size());
    settings.setValue("MainWindow/pos", pos());

    delete ui;
}

void MainWindow::pushMessage(QString msg, int type) {
    QLabel *l = new QLabel(this);
    l->setText(msg);

    QString css = "border-radius: 5px; border-width: 2px; border-style: solid; padding: 2px; ";
    switch (type) {
    case Info:
        css += "background-color: #eea; border-color: #cc9;";
        break;
    case Error:
        css += "background-color: #eaa; border-color: #c99; ";
        break;
    }
    l->setStyleSheet(css);

    ((QVBoxLayout*)ui->groupMessages->layout())->insertWidget(0, l);

    QTimer::singleShot(3000, this, SLOT(deleteLastMessage()));
}

void MainWindow::deleteLastMessage() {
    QVBoxLayout *layout = (QVBoxLayout*)ui->groupMessages->layout();
    delete layout->itemAt(layout->count()-1)->widget();
}

void MainWindow::getStreams(QString url) {
    QUrl urlGetStreams = QUrl("http://pirateplay.se/api/get_streams.xml");
    urlGetStreams.addQueryItem("url", url);
    getStreamsReply = qnam->get(QNetworkRequest(urlGetStreams));
    connect(getStreamsReply, SIGNAL(finished()), this, SLOT(getStreamsFinished()));

    pushMessage(QString::fromUtf8("Hämtar strömmar..."), Info);
}

void MainWindow::getStreamsFinished() {
    StreamTableModel *streamList = new StreamTableModel(getStreamsReply, this);

    if (streamList->rowCount() < 1) {
        pushMessage(QString::fromUtf8("Inga strömmar funna!"), Error);
        return;
    }

    SaveStreamDialog *streamDialog = new SaveStreamDialog(streamList, userSettings, this);
    if (streamDialog->exec() == QDialog::Accepted) {
        downloadStack->addDownload(streamDialog->getUrl(), streamDialog->getFileName());
        if(streamDialog->downloadSubs())
            downloadStack->addDownload(streamDialog->getSubUrl(), streamDialog->getSubFileName());
    }

    getStreamsReply->deleteLater();
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

    twitterWidgetMapper = new QDataWidgetMapper(this);
    twitterWidgetMapper->setModel(filterModel);
    twitterWidgetMapper->addMapping(ui->tweetText, 0, "text");
    twitterWidgetMapper->toFirst();
    connect(filterModel, SIGNAL(dataChanged(QModelIndex,QModelIndex)), twitterWidgetMapper, SLOT(toFirst()));
    enableTwitterButtons(0);
    connect(twitterWidgetMapper, SIGNAL(currentIndexChanged(int)), this, SLOT(enableTwitterButtons(int)));
    connect(ui->buttonPrevTweet, SIGNAL(clicked()), twitterWidgetMapper, SLOT(toPrevious()));
    connect(ui->buttonNextTweet, SIGNAL(clicked()), twitterWidgetMapper, SLOT(toNext()));
    connect(ui->tweetText, SIGNAL(clicked()), this, SLOT(openCurrentTweet()));
}

void MainWindow::openCurrentTweet() {
    QDesktopServices::openUrl(QUrl(twitterWidgetMapper->model()->data(twitterWidgetMapper->model()->index(twitterWidgetMapper->currentIndex(), 2)).toString()));
}

void MainWindow::tabChanged(int index) {
    if (index != 1) {
        writeUserSettings();
    }
}

void MainWindow::readUserSettings() {
    userSettings["player_cmd"] = settings.value("Location/player_cmd", "ffplay \"%0\"");
    userSettings["start_dir"] = settings.value("Location/start_dir", QDesktopServices::storageLocation(QDesktopServices::HomeLocation));
    ui->editPlayer->setText(userSettings["player_cmd"].toString());
    ui->editStartDir->setText(userSettings["start_dir"].toString());
}

void MainWindow::writeUserSettings() {
    settings.setValue("Location/player_cmd", ui->editPlayer->text());
    settings.setValue("Location/start_dir", ui->editStartDir->text());
    userSettings["player_cmd"] = settings.value("Location/player_cmd");
    userSettings["start_dir"] = settings.value("Location/start_dir");
}

void MainWindow::resetUserSettings() {
    settings.remove("Location/player_cmd");
    settings.remove("Location/start_dir");
    readUserSettings();
}
