#ifndef TEMPPLAYER_H
#define TEMPPLAYER_H

#include <QObject>
#include <QTemporaryFile>
#include <QNetworkAccessManager>

#include "download.h"

class TempPlayer : public QObject
{
    Q_OBJECT
public:
    explicit TempPlayer(QNetworkAccessManager *existingNam = 0, QObject *parent = 0);

    void play(const QString &url);
    void setTempDir(const QString &newDir);
    
private slots:
    void startPlaying();

private:
    //PirateNetworkAccessManager *nam;
    QNetworkAccessManager *nam;
    QString tempDir;
    Download *dl;
    QTemporaryFile tmpFile;
    
    void cleanUp();
};

#endif // TEMPPLAYER_H
