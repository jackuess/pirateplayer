#include "downloadhttp.h"

#include <QDebug>

DownloadHttp::DownloadHttp(QObject *parent, QNetworkAccessManager *qnam) :
    Download(parent)
{
    networkAccessManager = qnam;
}

void DownloadHttp::downloadToFile(QString url, QString filePath, bool resume) {
    localFile.setFileName(filePath);
    localFile.open(QIODevice::WriteOnly);

    networkReply = networkAccessManager->get(QNetworkRequest(QUrl(url)));
    connect(networkReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(httpDownloadProgress(qint64,qint64)));
    connect(networkReply, SIGNAL(readyRead()), this, SLOT(writeToFile()));
    connect(networkReply, SIGNAL(finished()), this, SLOT(reqFinished()));
}

void DownloadHttp::httpDownloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    emit downloadProgress(bytesReceived, bytesTotal);
}

QString DownloadHttp::getFileName() {
    return localFile.fileName();
}

void DownloadHttp::writeToFile() {
    localFile.write(networkReply->readAll());
}

void DownloadHttp::reqFinished() {
    QUrl redirectTarget = networkReply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

    localFile.close();

    if (redirectTarget.isEmpty()) {
        networkReply->deleteLater();
        emit finished();
    }
    else
        downloadToFile(redirectTarget.toString(), localFile.fileName());
}
