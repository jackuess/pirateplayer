#ifndef PIRATENETWORKREPLY_H
#define PIRATENETWORKREPLY_H

#include "rtmpsession.h"

#include <QNetworkReply>
#include <librtmp/rtmp.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

struct RtmpResume {
    int initialFrameType;
    uint32_t dSeek;
    char *metaHeader;
    char *initialFrame;
    uint32_t nMetaHeaderSize;
    uint32_t nInitialFrameSize;
    RtmpResume() : initialFrameType(0), dSeek(0), metaHeader(0), initialFrame(0), nMetaHeaderSize(0), nInitialFrameSize(0) {}
};
Q_DECLARE_METATYPE(RtmpResume)

class PirateNetworkReply : public QNetworkReply
{
    Q_OBJECT
public:
    PirateNetworkReply(QObject *parent = 0, QString rtmpUrl = "", RtmpResume resumeData = RtmpResume());
    ~PirateNetworkReply();

    qint64 bytesAvailable() const;
    bool isSequential() const;
    void abort();

protected:
    qint64 readData(char *data, qint64 maxSize);

private:
    RtmpSession *rtmpSession;
    RTMP * rtmp;
    ring_buffer *buffer;
    uint32_t rtmpBufferTime;
    bool started;
};

#endif // PIRATENETWORKREPLY_H
