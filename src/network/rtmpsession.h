#ifndef RTMPSESSION_H
#define RTMPSESSION_H

#include <librtmp/rtmp.h>
#include <librtmp/log.h>

#include <QThread>
#include <QWaitCondition>
#include <QMutex>

const int DEF_BUFTIME = (10 * 60 * 60 * 1000);
const int BUFFER_SIZE = 64 * 1024;
struct ring_buffer {
    qint64 nUsedBytes;
    qint64 offset;
    char *data;
    QWaitCondition notEmpty;
    QWaitCondition notFull;
    QMutex mutex;
};

class RtmpSession : public QThread
{
    Q_OBJECT
public:
    explicit RtmpSession(QObject *parent = 0, RTMP * r = 0, ring_buffer *buff = 0);
    ~RtmpSession();

    void run();
    void abort();

    quint64 size;
    
signals:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void readyRead();
    
public slots:

private:
    bool toAbort;
    RTMP *rtmp;
    ring_buffer *buffer;
};

#endif // RTMPSESSION_H
