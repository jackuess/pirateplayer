#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include "downloadlistmodel.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

class Download : public AbstractDownload
{
    Q_OBJECT
public:
    Download(QObject *parent, QNetworkAccessManager *n);
    ~Download();

    void abort();

protected slots:
    void startDownload();
    void onProgress(qint64 recieved, qint64 total);
    void onFinished();
    void writeData();
    void onNetworkError(QNetworkReply::NetworkError /*unused*/);

private:
    QNetworkAccessManager *nam;
    QNetworkReply *reply;
    QFile *outFile;
};

#endif // DOWNLOAD_H
