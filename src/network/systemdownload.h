#ifndef SYSTEMDOWNLOAD_H
#define SYSTEMDOWNLOAD_H

#include "downloadlistmodel.h"

#include <QProcess>
#include <QRegExp>
#include <QTime>

class SystemDownload : public AbstractDownload
{
    Q_OBJECT
public:
    SystemDownload(QObject *parent);
    ~SystemDownload();

    void abort();

    static QString ffmpegPrefix();
    static bool ffmpegIsInstalled();
    static bool installFfmpeg();

private:
    static QString FFMPEG_CMD;
    static QRegExp rxDuration;
    static QRegExp rxCurrTime;
    QProcess *program;
    QString stdErrBuffer;
    QTime duration;


protected slots:
    void startDownload();
    void capDuration();
    void capCurrentTime();
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif // SYSTEMDOWNLOAD_H
