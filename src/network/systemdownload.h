#ifndef SYSTEMDOWNLOAD_H
#define SYSTEMDOWNLOAD_H

#include "abstractdownload.h"

#include <QProcess>
#include <QRegExp>
#include <QTime>

class SystemDownload : public AbstractDownload
{
    Q_OBJECT
public:
    SystemDownload(QObject *parent, QUrl u);
    void downloadToFile(QString fileName);
    void abort();

private:
    static QString FFMPEG_CMD;
    static QRegExp rxDuration;
    static QRegExp rxCurrTime;
    QProcess *program;
    QString stdErrBuffer;
    QTime duration;

private slots:
    void capDuration();
    void capCurrentTime();
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
};

#endif // SYSTEMDOWNLOAD_H
