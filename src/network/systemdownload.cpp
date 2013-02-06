#include "systemdownload.h"

#include <QDebug>
#include <QCoreApplication>

QRegExp SystemDownload::rxDuration = QRegExp("Duration:\\s(\\d\\d):(\\d\\d):(\\d\\d)");

#ifndef UBUNTU_12_04
QRegExp SystemDownload::rxCurrTime = QRegExp("size=\\s*(\\d+)kB\\stime=(\\d\\d):(\\d\\d):(\\d\\d)");
#else
QRegExp SystemDownload::rxCurrTime = QRegExp("time=(\\d+)");
#endif

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
#elif defined Q_WS_MAC
    QString ffmpegName = QCoreApplication::applicationDirPath() + "/ffmpeg";
#else
    QString ffmpegName = "ffmpeg";
#endif

    if (url.scheme() == "mms")
        url.setScheme("mmsh"); //Ffmpeg needs this
    else if(url.toString().indexOf(".m3u8") > -1) {
#ifdef UBUNTU_12_04
        acodec = "aac";
        extra << "-bsf" << "aac_adtstoasc" << "-strict" << "experimental" << "-ab" << "325k";
#else
        extra << "-absf" << "aac_adtstoasc";
#endif
    }

    arguments << "-i" << url.toString() << "-vcodec" << vcodec << "-acodec" << acodec << "-y" << extra << fileName;

    program = new QProcess(this);
    program->start(ffmpegName, arguments);

    connect(program, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onFinished(int,QProcess::ExitStatus)));
    connect(program, SIGNAL(readyReadStandardError()), this, SLOT(capDuration()));
}

void SystemDownload::abort() {
    //program->close();
    program->terminate();
    //program->kill();
}

void SystemDownload::onFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if (exitCode == 0 && exitStatus == 0 && downloadProgress > 99) {
        status = AbstractDownload::Finished;
        downloadProgress = 100;
        emit progress();
    }
    else if (exitCode != 0 && exitStatus == 0)//else if (exitCode == 0 && exitStatus != 0)
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
#ifndef UBUNTU_12_04
        int kBytes = rxCurrTime.cap(1).toInt();
        QTime currTime = QTime(rxCurrTime.cap(2).toInt(), rxCurrTime.cap(3).toInt(), rxCurrTime.cap(4).toInt());
#else
        int kBytes = 0;
        QTime currTime = nullTime.addSecs(rxCurrTime.cap(1).toInt());
        qDebug() << rxCurrTime.cap(1) << currTime;
#endif

        if (currTime.secsTo(duration) < 2)
            downloadProgress = 100;
        else
            downloadProgress = (int)((double)currTime.secsTo(nullTime) / (double)duration.secsTo(nullTime) * 100);

        bytesRecieved = kBytes * 1024;
        stdErrBuffer = "";
        emit progress();
    }
}
