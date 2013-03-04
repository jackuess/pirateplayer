#include "systemdownload.h"

#include <QDebug>
#include <QCoreApplication>
#include <QDesktopServices>
#include <QFile>
#include <QFileInfo>

QRegExp SystemDownload::rxDuration = QRegExp("Duration:\\s(\\d\\d):(\\d\\d):(\\d\\d)");

QRegExp SystemDownload::rxCurrTime = QRegExp("size=\\s*(\\d+)kB\\stime=(\\d\\d):(\\d\\d):(\\d\\d)");

SystemDownload::SystemDownload(QObject *parent)
    :AbstractDownload(parent)
{
    program = new QProcess(this);
}

SystemDownload::~SystemDownload() {
    if (program->state() == QProcess::Running) {
        abort();
        program->waitForFinished();
    }
}

QString SystemDownload::ffmpegPrefix() {
#if defined Q_WS_MAC
    return QCoreApplication::applicationDirPath() + "/";
#elif defined Q_OS_ANDROID
    return QDesktopServices::storageLocation(QDesktopServices::DataLocation) + "/";
#else
    return "";
#endif
}

bool SystemDownload::ffmpegIsInstalled() {
    int ret = QProcess::execute(ffmpegPrefix() + "ffmpeg -version");
    return !ret;
}

bool SystemDownload::installFfmpeg() {
#if defined Q_OS_ANDROID
    QString ffmpegPath = ffmpegPrefix() + "ffmpeg";
    if (QFile::copy("assets:/ffmpeg", ffmpegPath))
        QFile::setPermissions(ffmpegPath, QFile::ExeOwner | QFile::ReadOwner);
    return ffmpegIsInstalled();
#else
    return false;
#endif
}

void SystemDownload::startDownload() {
    AbstractDownload::startDownload();

    QFileInfo file(outFileName);
    QStringList arguments;
    QStringList extra = QStringList();

    if (!ffmpegIsInstalled() && !installFfmpeg()) {
        qWarning() << "Fmpeg not installed!\nAborting download.";
        status = DownloadListModel::Error;
        emit statusChanged();
        return;
    }

    if (_url.scheme() == "mms")
        _url.setScheme("mmsh"); //Ffmpeg needs this
    else if (file.suffix() == "mp4" && _url.toString().indexOf(".m3u8") > -1) {
        extra << "-absf" << "aac_adtstoasc";
    }

    arguments << "-i" << _url.toString() << "-vcodec" << "copy" << "-acodec" << "copy" << "-y" << extra << outFileName;

    program->start(ffmpegPrefix() + "ffmpeg", arguments);

    connect(program, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(onFinished(int,QProcess::ExitStatus)));
    connect(program, SIGNAL(readyReadStandardError()), this, SLOT(capDuration()));
}

void SystemDownload::abort() {
    program->write("q");
}

void SystemDownload::onFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    if (exitCode == 0 && exitStatus == 0 && downloadProgress > 99) {
        status = DownloadListModel::Finished;
        downloadProgress = 100;
        emit progress();
    }
    else if (exitCode == 0 && exitStatus == 0)//else if (exitCode == 0 && exitStatus != 0)
        status = DownloadListModel::Aborted;
    else
        status = DownloadListModel::Error;

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
        int kBytes = rxCurrTime.cap(1).toInt();
        QTime currTime = QTime(rxCurrTime.cap(2).toInt(), rxCurrTime.cap(3).toInt(), rxCurrTime.cap(4).toInt());

        if (duration.secsTo(nullTime) == 0)
            downloadProgress = -1;
        else if (currTime.secsTo(duration) < 2)
            downloadProgress = 100;
        else
            downloadProgress = (int)((double)currTime.secsTo(nullTime) / (double)duration.secsTo(nullTime) * 100);

        bytesRecieved = kBytes * 1024;
        stdErrBuffer = "";
        emit progress();
    }
}
