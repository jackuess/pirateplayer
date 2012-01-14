#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtGui/QClipboard>
#include <QtCore/QRegExp>
#include <QDebug>
#include <QFileDialog>
#include <QUrl>
#include <QtNetwork/QNetworkReply>
#include <QMessageBox>
#include <QtXml>
#include <QDesktopServices>
#include <QVBoxLayout>
#include <QtWebKit>
#include <qglobal.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    networkAccessManager = new PirateNetworkAccessManager(this);

    setWindowIcon(QIcon(":/data/16x16/pirateplayer.png"));

    ui->setupUi(this);

    ui->browser->page()->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    ui->browser->settings()->globalSettings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    ui->browser->load(QUrl("http://pirateplay.se/playbrowser/browser.html"));
    connect(ui->browser->page(), SIGNAL(linkClicked(QUrl)), this, SLOT(linkClicked(QUrl)));

    videoPlayer = new PirateVideoPlayer(ui->tabVideoPlayer, networkAccessManager);
    ui->tabVideoPlayer->layout()->addWidget(videoPlayer);

    QRegExp rx("http://*");
    rx.setPatternSyntax(QRegExp::Wildcard);
    QClipboard *clipboard = QApplication::clipboard();
    if (rx.exactMatch(clipboard->text()))
        ui->lineEdit_URL->setText(clipboard->text());

    //layout()->setSizeConstraint(QLayout::SetFixedSize);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete networkAccessManager;
}

void MainWindow::linkClicked(QUrl url) {
    ui->lineEdit_URL->setText(url.toString());
}

void MainWindow::on_pushButton_Minimize_clicked(bool checked) {
    if (checked) {
        ui->browser->hide();
        QApplication::processEvents();
        adjustSize();
    }
    else
        ui->browser->show();
}

void MainWindow::on_pushButton_Fetch_clicked()
{
    QUrl url("http://pirateplay.se/generate;application.xml");
    QPair <QString , QString>urlArg;
    urlArg.first = "url";
    urlArg.second = ui->lineEdit_URL->text();
    QPair <QString , QString>libRtmpArg;
    libRtmpArg.first = "librtmp";
    libRtmpArg.second = "1";
    QPair <QString , QString>metaArg;
    //libRtmpArg.first = "metaincmd";
    //libRtmpArg.second = "0";
    QList<QPair<QString, QString> > query;
    query.insert(2, urlArg);
    query.insert(1, libRtmpArg);
    //query.insert(0, metaArg);
    url.setQueryItems(query);
    QNetworkRequest req;
    req.setUrl(url);
    req.setRawHeader("User-Agent", "Pirateplayer");
    networkAccessManager->get(req);
    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fetchFinished(QNetworkReply*)));

    ui->pushButton_Fetch->setEnabled(false);
    ui->lineEdit_URL->setEnabled(false);
}

void MainWindow::on_pushButton_Download_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Spara videoström", QDesktopServices::storageLocation(QDesktopServices::HomeLocation), "Flashvideo (*.flv)", 0, QFileDialog::DontConfirmOverwrite);

    if (filePath != "") {
        QHash<QString, QVariant> userData = ui->comboBox_Stream->itemData(ui->comboBox_Stream->currentIndex()).toHash();

        DownloadWidget *downloadWidget = new DownloadWidget(ui->tabDownloads, networkAccessManager);
        connect(downloadWidget, SIGNAL(kill()), this, SLOT(killDownloadWidget()));
        ((QVBoxLayout*)ui->tabDownloads->layout())->insertWidget(ui->tabDownloads->layout()->count()-1, downloadWidget);
        downloadWidget->startDownload(userData["url"].toString(), userData.value("subtitles", "").toString(), filePath, userData["referer"].toString());

        ui->comboBox_Stream->clear();
        ui->comboBox_Stream->setEnabled(false);
        ui->pushButton_Download->setEnabled(false);
        ui->pbPlay->setEnabled(false);
    }
}

void MainWindow::on_pbPlay_clicked() {
    QHash<QString, QVariant> userData = ui->comboBox_Stream->itemData(ui->comboBox_Stream->currentIndex()).toHash();
    videoPlayer->playUrl(userData["url"].toString());
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::fetchFinished(QNetworkReply *reply) {
    QDomDocument ppReplyDoc;
    ppReplyDoc.setContent(reply->readAll());
    QDomNodeList nodeList = ppReplyDoc.elementsByTagName("cmd");
    QString title;
    QHash<QString, QVariant> userData;
    for(int i=0; i<nodeList.count(); i++) {
        if(nodeList.item(i).attributes().contains("quality"))
            title = nodeList.item(i).attributes().namedItem("quality").toAttr().value();
        else
            title = QString("Ström %1").arg(i+1);

        if(nodeList.item(i).attributes().contains("subtitles"))
            userData["subtitles"] = nodeList.item(i).attributes().namedItem("subtitles").toAttr().value();
        userData["url"] = nodeList.item(i).toElement().text();
        userData["referer"] = reply->request().url().queryItemValue("url");

        ui->comboBox_Stream->addItem(title, userData);
    }
    reply->deleteLater();

    ui->pushButton_Download->setEnabled(true);
    ui->pbPlay->setEnabled(true);
    ui->comboBox_Stream->setEnabled(true);
    ui->pushButton_Fetch->setEnabled(true);
    ui->lineEdit_URL->setEnabled(true);

    disconnect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fetchFinished(QNetworkReply*)));
}

void MainWindow::killDownloadWidget() {
    delete QObject::sender();
}
