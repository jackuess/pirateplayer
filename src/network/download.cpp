#include "download.h"

#include <QNetworkRequest>
#include <QDebug>

Download::Download(QObject *parent, QNetworkAccessManager *n):
    AbstractDownload(parent)
{
    nam = n;
}

Download::~Download() {
    ;
}

void Download::startDownload() {
    AbstractDownload::startDownload();

    reply = nam->get(QNetworkRequest(_url));

    outFile = new QFile(outFileName, this);
    outFile->open(QIODevice::WriteOnly);

    connect(reply, SIGNAL(finished()), this, SLOT(onFinished()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(onProgress(qint64,qint64)));
    connect(reply, SIGNAL(readyRead()), this, SLOT(writeData()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(onNetworkError(QNetworkReply::NetworkError)));
}

void Download::abort() {
    if (status == DownloadListModel::Downloading) {
        reply->abort();
        status = DownloadListModel::Aborted;
        emit statusChanged();
    }
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

    if (status == DownloadListModel::Downloading) {
        status = DownloadListModel::Finished;
        downloadProgress = 100;
        emit progress();
        emit statusChanged();
    }
}

void Download::onNetworkError(QNetworkReply::NetworkError e) {
    qDebug() << reply->errorString();
    reply->deleteLater();
    status = DownloadListModel::Error;
    emit statusChanged();
}

void Download::writeData() {
    outFile->write(reply->read(reply->bytesAvailable()));
}
