#ifndef PIRATENETWORKREPLY_H
#define PIRATENETWORKREPLY_H

#include <QNetworkReply>
#include <librtmp/rtmp.h>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#define DEF_BUFTIME     (10 * 60 * 60 * 1000)   /* 10 hours default */
const int BUFFER_SIZE = 64 * 1024;

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

    RTMP * rtmp;
    uint32_t rtmpBufferTime;
    char * buffer;
    qint64 offset;
    qint64 bytesToRead;
    qint64 bytesReceived;
    bool toAbort;
};

#endif // PIRATENETWORKREPLY_H
