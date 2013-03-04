#ifndef ARCHIVEEXTRACTOR_H
#define ARCHIVEEXTRACTOR_H

#include <QObject>
#include <QIODevice>
#include <QNetworkReply>

#include <archive.h>
#include <archive_entry.h>

#ifdef Q_WS_MAC
  #define off64_t off_t
#endif

class ArchiveExtractor : public QObject
{
    Q_OBJECT
public:
    explicit ArchiveExtractor(QObject *parent = 0);
    
signals:
    void finished();

public slots:
    void extractToDir(QByteArray *data, const QString &outDir = "");
    void extractToDir(QIODevice *data, const QString &outDir = "");
    void extractToDir(QNetworkReply *data, const QString &outDir = "");

private slots:
    void extractToDirQBA(QByteArray *data, const QString &outDir = "");
    void extractToDirQIO(QIODevice *data, const QString &outDir = "");

private:
    int copyData(struct archive *ar, struct archive *aw);
};

#endif // ARCHIVEEXTRACTOR_H
