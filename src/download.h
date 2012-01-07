#ifndef DOWNLOAD_H
#define DOWNLOAD_H

#include <QObject>

class Download : public QObject {
    Q_OBJECT

public:
    Download(QObject *parent) : QObject(parent) {}
    virtual ~Download() {}

    virtual void downloadToFile(QString url, QString filePath, bool resume = false) = 0;
    virtual QString getFileName() = 0;

signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void finished();
};

#endif // DOWNLOAD_H
