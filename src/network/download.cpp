#include "download.h"
#include "abstractdownload.h"

#include <QNetworkRequest>
#include <QDebug>

Download::Download(QObject *parent, QNetworkAccessManager *n, QUrl u):
    AbstractDownload(parent, u)
{
    nam = n;
}

Download::~Download() {
    ;
}

void Download::downloadToFile(QString outFileName)
{
    AbstractDownload::downloadToFile(outFileName);

    QNetworkRequest req;
    req.setUrl(url);

    reply = nam->get(req);

    outFile = new QFile(this->outFileName, this);
    outFile->open(QIODevice::WriteOnly);

    connect(reply, SIGNAL(finished()), this, SLOT(onFinished()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onProgress(qint64,qint64)));
    connect(reply, SIGNAL(readyRead()), this, SLOT(writeData()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetworkError(QNetworkReply::NetworkError)));
}

void Download::abort() {
    reply->abort();
    status = AbstractDownload::Aborted;
    emit statusChanged();
}

void Download::onProgress(qint64 recieved, qint64 total) {
    if (total > 0)
        downloadProgress = (int)((double)recieved/(double)total*100);
    else
        downloadProgress = -1;
    bytesRecieved = recieved;
    emit progress();
}

void Download::onFinished() {
    outFile->close();
    reply->deleteLater();

    if (status == AbstractDownload::Downloading) {
        status = AbstractDownload::Finished;
        downloadProgress = 100;
        emit progress();
        emit statusChanged();
    }
}

void Download::onNetworkError(QNetworkReply::NetworkError e) {
    qDebug() << reply->errorString();
    delete reply;
    status = AbstractDownload::Error;
    emit statusChanged();
}

void Download::writeData() {
    outFile->write(reply->read(reply->bytesAvailable()));
}
