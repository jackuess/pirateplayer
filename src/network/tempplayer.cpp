#include "tempplayer.h"

#include "../pirateplayercontext.h"
#include "piratenetworkaccessmanager.h"

#include <QDir>
#include <QDebug>
#include <QMessageBox>
#include <QApplication>
#include <QDesktopWidget>

TempPlayer::TempPlayer(QNetworkAccessManager *existingNam, QObject *parent) :
    QObject(parent)
{
    if (existingNam == 0)
        nam = new PirateNetworkAccessManager(this);
    else
        nam = existingNam;

    tempDir = QDir::tempPath();
    dl = new Download(this, nam);
}

void TempPlayer::setTempDir(const QString &newDir) {
    tempDir = newDir;
}

void TempPlayer::play(const QString &url) {
    tmpFile.setFileTemplate(tempDir + "/XXXXXX.flv");
    tmpFile.open();
    qDebug() << "Starting download to temporary file" << tmpFile.fileName();
    dl->downloadToFile(url, tmpFile.fileName());
    connect(dl, SIGNAL(progress()), SLOT(startPlaying()));
}

void TempPlayer::startPlaying() {
    if (dl->getProgress() > 1 || (dl->getProgress() < 0 && dl->getBytesRecieved() > 1024*1024)) {
        QString url("file://" + dl->getFileName());
        PirateplayerContext::openUrl(url);
        disconnect(dl, SIGNAL(progress()), this, SLOT(startPlaying()));
        cleanUp();
    }
}

void TempPlayer::cleanUp() {
    QMessageBox msgBox;
    msgBox.show();
    msgBox.setText(QString::fromUtf8("Uppspelning färdig!"));
#ifdef Q_OS_ANDROID
    QWidget *w = QApplication::desktop();
    msgBox.move(w->width()/2-msgBox.width()/2, w->height()/2-msgBox.height()/2);
#endif
    msgBox.exec();

    dl->abort();
}
