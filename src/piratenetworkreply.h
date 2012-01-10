#ifndef PIRATENETWORKREPLY_H
#define PIRATENETWORKREPLY_H

#include "rtmpsession.h"

#include <QNetworkReply>
#include <librtmp/rtmp.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

class PirateNetworkReply : public QNetworkReply
{
    Q_OBJECT
public:
    PirateNetworkReply(QObject *parent = 0, QString rtmpUrl = "");
    ~PirateNetworkReply();

    qint64 bytesAvailable() const;
    bool isSequential() const;
    void abort();

protected:
    qint64 readData(char *data, qint64 maxSize);

private:
    void fillBuffer();

    RtmpSession *rtmpSession;
    RTMP * rtmp;
    ring_buffer *buffer2;
    uint32_t rtmpBufferTime;
    char * buffer;
    qint64 offset;
    qint64 bytesToRead;
    qint64 bytesReceived;
    bool toAbort;
};

#endif // PIRATENETWORKREPLY_H
