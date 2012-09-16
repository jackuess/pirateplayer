#ifndef ABSTRACTDOWNLOAD_H
#define ABSTRACTDOWNLOAD_H

#include <QObject>
#include <QUrl>

class AbstractDownload : public QObject
{
    Q_OBJECT
public:
    explicit AbstractDownload(QObject *parent, QUrl u) { downloadProgress = 0; status = NotStarted; url = u; }

    virtual void downloadToFile(QString fileName) { status = Downloading; emit statusChanged(); outFileName = fileName; }
    virtual void abort() {}

    QString getUrl() { return url.toString(); }
    QString getFileName() { return outFileName; }
    int getProgress() { return downloadProgress; }
    int getStatus() { return status; }

    enum Status {
        NotStarted = 0,
        Downloading,
        Finished,
        Error,
        Aborted
    };
    
signals:
    void progress();
    void statusChanged();
    
public slots:
    
protected:
    QString outFileName;
    QUrl url;
    int downloadProgress;
    int status;
};

#endif // ABSTRACTDOWNLOAD_H
