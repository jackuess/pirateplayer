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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    networkAccessManager = new PirateNetworkAccessManager(this);

    ui->setupUi(this);

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

void MainWindow::on_pushButton_Fetch_clicked()
{
    fetchUrl = ui->lineEdit_URL->text();
    QUrl url("http://pirateplay.se/generate;application.xml");
    QPair <QString , QString>urlArg;
    urlArg.first = "url";
    urlArg.second = fetchUrl;
    QPair <QString , QString>libRtmpArg;
    libRtmpArg.first = "librtmp";
    libRtmpArg.second = "1";
    QList<QPair<QString, QString> > query;
    query.insert(1, urlArg);
    query.insert(0, libRtmpArg);
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
    QString filePath = QFileDialog::getSaveFileName(this, "Spara videoström", QDesktopServices::storageLocation(QDesktopServices::HomeLocation), "Flashvideo (*.flv)");
    if (filePath != "") {
        QString url = ui->comboBox_Stream->itemData(ui->comboBox_Stream->currentIndex()).toString();

        DownloadWidget *downloadWidget = new DownloadWidget(ui->tabDownloads, networkAccessManager);
        connect(downloadWidget, SIGNAL(kill()), this, SLOT(killDownloadWidget()));
        ui->tabDownloads->layout()->addWidget(downloadWidget);
        downloadWidget->startDownload(url, filePath, fetchUrl);

        ui->comboBox_Stream->clear();
        ui->comboBox_Stream->setEnabled(false);
        ui->pushButton_Download->setEnabled(false);
        ui->pbPlay->setEnabled(false);
    }
}

void MainWindow::on_pbPlay_clicked() {
    videoPlayer->playUrl(ui->comboBox_Stream->itemData(ui->comboBox_Stream->currentIndex()).toString());
    ui->tabWidget->setCurrentIndex(1);
}

struct Stream {
    QString title;
    QString subtitles;
    QString url;
};

void MainWindow::fetchFinished(QNetworkReply *reply) {
    QDomDocument ppReplyDoc;
    ppReplyDoc.setContent(reply->readAll());
    QDomNodeList nodeList = ppReplyDoc.elementsByTagName("cmd");
    Stream stream;
    for(int i=0; i<nodeList.count(); i++) {
        if(nodeList.item(i).attributes().contains("quality"))
            stream.title = nodeList.item(i).attributes().namedItem("quality").toAttr().value();
        else
            stream.title = QString("Ström %1").arg(i+1);
        stream.url = nodeList.item(i).toElement().text();
        ui->comboBox_Stream->insertItem(i+1, stream.title, stream.url);
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
