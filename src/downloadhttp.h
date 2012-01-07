#ifndef DOWNLOADHTTP_H
#define DOWNLOADHTTP_H

#include "download.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

class DownloadHttp : public Download
{
    Q_OBJECT

public:
    DownloadHttp(QObject *parent, QNetworkAccessManager *qnam);

    void downloadToFile(QString url, QString filePath, bool resume = false);
    QString getFileName();

private:
    QNetworkAccessManager *networkAccessManager;
    QNetworkReply *networkReply;
    QFile localFile;

private slots:
    void httpDownloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void writeToFile();
    void reqFinished();
};

#endif // DOWNLOADHTTP_H
