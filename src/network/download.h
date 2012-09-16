#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "abstractdownload.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

class Download : public AbstractDownload
{
    Q_OBJECT
public:
    Download(QObject *parent, QNetworkAccessManager *n, QUrl u);
    ~Download();

    void downloadToFile(QString outFileName);
    void abort();

private slots:
    void onProgress(qint64 recieved, qint64 total);
    void onFinished();
    void writeData();
    void onNetworkError(QNetworkReply::NetworkError e);

private:
    QNetworkAccessManager *nam;
    QNetworkReply *reply;
    QFile *outFile;
};

#endif // DOWNLOAD_H
