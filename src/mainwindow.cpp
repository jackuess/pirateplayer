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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    networkAccessManager = new PirateNetworkAccessManager(this);

    QRegExp rx("http://*");
    rx.setPatternSyntax(QRegExp::Wildcard);
    QClipboard *clipboard = QApplication::clipboard();
    if (rx.exactMatch(clipboard->text()))
        ui->lineEdit_URL->setText(clipboard->text());
}

MainWindow::~MainWindow()
{
    delete ui;
    delete networkAccessManager;
}

void MainWindow::on_pushButton_Download_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Spara videoström", QDesktopServices::storageLocation(QDesktopServices::HomeLocation), "Flashvideo (*.flv)");
    if (filePath != "") {
        //file = QFile(filePath);
        file.setFileName(filePath);
        file.open(QIODevice::WriteOnly);

        QString cmd = ui->comboBox_Stream->itemData(ui->comboBox_Stream->currentIndex()).toString();
        cmd.remove(0, 8);
        QRegExp rxUrl(" (?:-r|--rtmp) ([^ ]+)");
        rxUrl.indexIn(cmd);
        cmd.remove(rxUrl);
        QString rtmpUrl = rxUrl.cap(1);
        cmd.replace(QRegExp("-W |--swfVfy "), "swfVfy=1 swfUrl=");
        cmd.replace(QRegExp("-y |--playpath "), "playpath=");
        cmd.replace(QRegExp("-v |--live "), "live=");
        cmd.replace(QRegExp("-a |--app "), "app=");
        cmd.remove(QRegExp("-o -|--flv -"));
        qDebug() << cmd;
        rtmpUrl += cmd;
        rtmpUrl.remove('"');
        qDebug() << rtmpUrl;

        networkReply = networkAccessManager->get(QNetworkRequest(QUrl(rtmpUrl)));
        connect(networkReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
        connect(networkReply, SIGNAL(readyRead()), this, SLOT(writeToFile()));
        connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ppDownloadFinished(QNetworkReply*)));

        ui->pushButton_Download->setEnabled(false);
        ui->comboBox_Stream->setEnabled(false);
    };
}

void MainWindow::downloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    ui->progressBar->setValue((int)((double)bytesReceived/(double)bytesTotal*100.0));
}

void MainWindow::writeToFile() {
    file.write(networkReply->read(networkReply->bytesAvailable()));
}

void MainWindow::ppDownloadFinished(QNetworkReply *reply) {
    QMessageBox::information(this, "Nedladdning", "Nedladdning klar!");
    reply->deleteLater();
    file.close();

    ui->pushButton_Fetch->setEnabled(true);
    ui->lineEdit_URL->setEnabled(true);
    ui->comboBox_Stream->clear();

    disconnect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(ppDownloadFinished(QNetworkReply*)));
}

void MainWindow::on_pushButton_Fetch_clicked()
{
    QUrl url("http://pirateplay.se/generate;application.xml");
    QPair <QString , QString>pair;
    pair.first = "url";
    pair.second = ui->lineEdit_URL->text();
    QList<QPair<QString, QString> > query;
    query.insert(0, pair);
    url.setQueryItems(query);
    QNetworkRequest req;
    req.setUrl(url);
    req.setRawHeader("User-Agent", "Pirateplayer");
    networkReply = networkAccessManager->get(req);
    connect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fetchFinished(QNetworkReply*)));
    connect(networkReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));

    ui->pushButton_Fetch->setEnabled(false);
    ui->lineEdit_URL->setEnabled(false);
}

void MainWindow::fetchFinished(QNetworkReply *reply) {
    QDomDocument ppReplyDoc;
    ppReplyDoc.setContent(reply->readAll());
    QDomNodeList nodeList = ppReplyDoc.elementsByTagName("cmd");
    for(int i=0; i<nodeList.count(); i++) {
        QStringList streamPair = nodeList.item(i).toElement().text().split("\n");
        ui->comboBox_Stream->insertItem(i+1, streamPair[0], streamPair[1]);
    }
    reply->deleteLater();

    ui->progressBar->setValue(0);
    ui->pushButton_Download->setEnabled(true);
    ui->comboBox_Stream->setEnabled(true);

    disconnect(networkAccessManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(fetchFinished(QNetworkReply*)));
}
