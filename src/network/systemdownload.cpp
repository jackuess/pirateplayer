#include "systemdownload.h"

#include <QDebug>

QRegExp SystemDownload::rxDuration = QRegExp("Duration:\\s(\\d\\d):(\\d\\d):(\\d\\d)");
QRegExp SystemDownload::rxCurrTime = QRegExp("time=(\\d\\d):(\\d\\d):(\\d\\d)");

SystemDownload::SystemDownload(QObject *parent, QUrl u)
    :AbstractDownload(parent, u)
{
    ;
}

void SystemDownload::downloadToFile(QString fileName) {
    AbstractDownload::downloadToFile(fileName);
    QStringList arguments;
    QStringList extra = QStringList();

    QString vcodec = "copy";
    QString acodec = "copy";
#ifdef UBUNTU_12_04
    QString ffmpegName = "avconv";
#else
    QString ffmpegName = "ffmpeg";
#endif

    if (url.scheme() == "mms")
        url.setScheme("mmsh"); //Ffmpeg needs this
    else if(url.toString().indexOf(".m3u8") > -1) {

#ifdef UBUNTU_12_04
        acodec = "aac"
        extra << "-bsf" << "aac_adtstoasc" << "-strict" << "experimental" << "-ab" << "325k";
#else
        extra << "-absf" << "aac_adtstoasc";
#endif

    }

    arguments << "-i" << url.toString() << "-vcodec" << vcodec << "-acodec" << acodec << "-y";
    arguments.append(extra);
//    if (url.toString().indexOf(".m3u8") > -1)
//        arguments << "-absf" << "aac_adtstoasc"; //Ffmpeg needs this for HLS-downloads

    arguments << fileName;

    program = new QProcess(this);

#ifdef USE_AVCONV
    program->start("avconv", arguments);
#else
    program->start(ffmpegName, arguments);
#endif

    connect(program, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onFinished(int,QProcess::ExitStatus)));
    connect(program, SIGNAL(readyReadStandardError()), this, SLOT(capDuration()));
}

void SystemDownload::abort() {
    program->close();
}

void SystemDownload::onFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if (exitCode == 0 && exitStatus == 0) {
        status = AbstractDownload::Finished;
        downloadProgress = 100;
        emit progress();
    }
    else if (exitCode == 0 && exitStatus != 0)
        status = AbstractDownload::Aborted;
    else
        status = AbstractDownload::Error;

    emit statusChanged();
}

void SystemDownload::capDuration() {
    stdErrBuffer += program->readAllStandardError();
    int pos = rxDuration.lastIndexIn(stdErrBuffer);
    if (pos > -1) {
        duration = QTime(rxDuration.cap(1).toInt(), rxDuration.cap(2).toInt(), rxDuration.cap(3).toInt());
        stdErrBuffer = "";
        disconnect(program, SIGNAL(readyReadStandardError()), this, SLOT(capDuration()));
        connect(program, SIGNAL(readyReadStandardError()), this, SLOT(capCurrentTime()));
    }
}

void SystemDownload::capCurrentTime() {
    stdErrBuffer += program->readAllStandardError();
    int pos = rxCurrTime.lastIndexIn(stdErrBuffer);
    if (pos > -1) {
        QTime nullTime = QTime(0, 0, 0);
        QTime currTime = QTime(rxCurrTime.cap(1).toInt(), rxCurrTime.cap(2).toInt(), rxCurrTime.cap(3).toInt());
        downloadProgress = (int)((double)currTime.secsTo(nullTime) / (double)duration.secsTo(nullTime) * 100);
        stdErrBuffer = "";
        emit progress();
    }
}
