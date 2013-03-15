#include "mediaplayerinterface.h"
#include "network/tempplayer.h"

#include <QDesktopServices>
#include <QProcess>
#include <QInputDialog>

MediaPlayerInterface::MediaPlayerInterface(QObject *parent) :
    QObject(parent)
{
#if defined( Q_OS_ANDROID ) || defined( EMULATE_ANDROID )
    nam = new QNetworkAccessManager(this);
#endif
    tempDir = "";
}

void MediaPlayerInterface::setNam(QNetworkAccessManager *newNam) {
#if defined( Q_OS_ANDROID ) || defined( EMULATE_ANDROID )
    delete nam;
    nam = newNam;
#endif
}

void MediaPlayerInterface::setTempDir(const QString &newTempDir) {
    tempDir = newTempDir;
}

void MediaPlayerInterface::play(const QString &url, const QString &subsUrl, const QString defaultCmdTemplate) {
    _url = url;
    _subsUrl = subsUrl;

#if defined( Q_OS_ANDROID ) || defined( EMULATE_ANDROID )
    Q_UNUSED( defaultCmdTemplate )
    if (url.startsWith("rtmp"))
        playTemp();
    else
        playNative();
#elif defined( Q_OS_MAC )
    if (url.contains(".m3u8"))
        playNative();
    else
        playUsingCmd(defaultCmdTemplate);
#else
    playUsingCmd(defaultCmdTemplate);
#endif
}

void MediaPlayerInterface::playNative() {
//#ifdef Q_OS_MAC
//    playUsingCmd("open -a \"QuickTime Player\" \"%0\"");
//#else
    QDesktopServices::openUrl(QUrl(_url));
//#endif
}

void MediaPlayerInterface::playTemp() {
#if defined( Q_OS_ANDROID ) || defined( EMULATE_ANDROID )
    TempPlayer *tmpPlayer = new TempPlayer(nam, this);
    if (tempDir != "")
        tmpPlayer->setTempDir(tempDir);
    tmpPlayer->play(_url);
#endif
}

void MediaPlayerInterface::playUsingCmd(const QString &cmdTemplate) {
    QProcess *cmd = new QProcess(this);
    bool ok = false;
    QString newCmdTemplate = QInputDialog::getText((QWidget*)this->parent(), tr("Ange uppspelningskommando"),
                                            QString::fromUtf8("Uppspelningskommando:\n    %0 ersätts med videoaddress.\n    %1 ersätts med undertextaddress."), QLineEdit::Normal,
                                            cmdTemplate, &ok);

    if (ok && !newCmdTemplate.isEmpty())
        cmd->start(newCmdTemplate.arg(_url, _subsUrl));
}
