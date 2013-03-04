#ifndef TEMPPLAYER_H
#define TEMPPLAYER_H

#include <QObject>
#include <QTemporaryFile>

#include "download.h"
#include "piratenetworkaccessmanager.h"

class TempPlayer : public QObject
{
    Q_OBJECT
public:
    explicit TempPlayer(PirateNetworkAccessManager *existingNam = 0, QObject *parent = 0);

    void play(const QString &url);
    void setTempDir(const QString &newDir);
    
private slots:
    void startPlaying();

private:
    PirateNetworkAccessManager *nam;
    QString tempDir;
    Download *dl;
    QTemporaryFile tmpFile;
    
    void cleanUp();
};

#endif // TEMPPLAYER_H
