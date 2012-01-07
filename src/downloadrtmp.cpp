#include "downloadrtmp.h"

#include <QDebug>

DownloadRtmp::DownloadRtmp(QObject *parent) :
    Download(parent)
{
    rtmpDump = new QProcess(this);
    rxProgress.setPattern("(\\d+\\.\\d+) kB.+(\\d+\\.\\d)%");
}

DownloadRtmp::~DownloadRtmp() {
    rtmpDump->terminate();
    rtmpDump->waitForFinished();
    delete rtmpDump;
}

void DownloadRtmp::downloadToFile(QString url, QString filePath, bool resume) {
    url = url.replace(" -o -", " -o \"" + filePath + "\"").replace(" -o \"-\"", " -o \"" + filePath + "\"");
    if (resume)
        url += " --resume";
    rtmpDump->start(url);
    connect(rtmpDump, SIGNAL(finished(int)), this, SLOT(downloadFinished()));
    connect(rtmpDump, SIGNAL(readyReadStandardError()), this, SLOT(readProgress()));
    fileName = filePath;
}

QString DownloadRtmp::getFileName() {
    return fileName;
}

void DownloadRtmp::downloadFinished() {
    emit finished();
}

void DownloadRtmp::readProgress() {
    if (rxProgress.indexIn(rtmpDump->readAllStandardError()) > -1) {
        qint64 bytesRecieved = (int) (rxProgress.cap(1).toDouble() * 1000.0);
        emit downloadProgress(bytesRecieved, (int) (bytesRecieved / (rxProgress.cap(2).toDouble() / 100.0)));
    }
}
