#ifndef DOWNLOADRTMP_H
#define DOWNLOADRTMP_H

#include "download.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QProcess>
#include <QRegExp>

class DownloadRtmp : public Download
{
    Q_OBJECT

public:
    DownloadRtmp(QObject *parent);
    ~DownloadRtmp();

    void downloadToFile(QString url, QString filePath, bool resume = false);
    QString getFileName();

private:
    QProcess *rtmpDump;
    QRegExp rxProgress;
    QString fileName;

private slots:
    void downloadFinished();
    void readProgress();
};

#endif // DOWNLOADRTMP_H
