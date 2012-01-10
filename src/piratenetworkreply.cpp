#include <QDebug>
#include <QTimer>

#include "piratenetworkreply.h"

int
init_fitta()
{
#ifdef WIN32
  WORD version;
  WSADATA wsaData;

  version = MAKEWORD(1, 1);
  return (WSAStartup(version, &wsaData) == 0);
#else
  return TRUE;
#endif
}

inline void
cleanup_sockets()
{
#ifdef WIN32
  WSACleanup();
#endif
}

PirateNetworkReply::PirateNetworkReply(QObject *parent, QString rtmpUrl) :
    QNetworkReply(parent)
{
    toAbort = false;
    bytesReceived = 0;
    rtmpBufferTime = DEF_BUFTIME;

    init_fitta();

    buffer = new char[BUFFER_SIZE + 2];
    rtmp = RTMP_Alloc();
    RTMP_Init(rtmp);

    buffer2 = new ring_buffer;
    buffer2->nUsedBytes = 0;
    buffer2->offset = 0;

    QByteArray ba = rtmpUrl.toLocal8Bit();
    if (RTMP_SetupURL(rtmp, ba.data())) {
        RTMP_SetBufferMS(rtmp, rtmpBufferTime);
        if(RTMP_Connect(rtmp, NULL)) {
            if(RTMP_ConnectStream(rtmp, 0)) {
                open(ReadOnly);
                //fillBuffer();
                rtmpSession = new RtmpSession(this, rtmp, buffer2);
                connect(rtmpSession, SIGNAL(downloadProgress(qint64,qint64)), this, SIGNAL(downloadProgress(qint64,qint64)));
                connect(rtmpSession, SIGNAL(readyRead()), this, SIGNAL(readyRead()));
                connect(rtmpSession, SIGNAL(finished()), this, SIGNAL(finished()));
                rtmpSession->start();
            }
        }
    }
}

PirateNetworkReply::~PirateNetworkReply() {
    delete[] buffer;
    rtmpSession->abort();
    rtmpSession->wait();
    delete rtmpSession;
    delete buffer2;
    RTMP_Free(rtmp);
}

void PirateNetworkReply::fillBuffer() {
    bytesToRead = RTMP_Read(rtmp, buffer, BUFFER_SIZE);
    offset = 0;
    if (!toAbort && bytesToRead > 0 && RTMP_IsConnected(rtmp) && !RTMP_IsTimedout(rtmp)) {
        bytesReceived += bytesToRead;

        QTimer::singleShot( 0, this, SIGNAL(readyRead()) );

        double duration = RTMP_GetDuration(rtmp);

        // Make sure we claim to have enough buffer time!
        if (rtmpBufferTime < (duration * 1000.0)) {
            rtmpBufferTime = (uint32_t) (duration * 1000.0) + 5000;   // extra 5sec to make sure we've got enough

            RTMP_SetBufferMS(rtmp, rtmpBufferTime);
            RTMP_UpdateBufferMS(rtmp);
        }

        if (rtmp->m_read.timestamp > 0)
            emit downloadProgress(bytesReceived, 1000*duration * (bytesReceived / rtmp->m_read.timestamp));

        //For testing: seek to 4 min into the stream
        //if((int) percent < 10)
        //    RTMP_SendSeek(r, 4*60*1000);
    }
    else
        emit finished();
}

bool PirateNetworkReply::isSequential() const {
    return false;
}

qint64 PirateNetworkReply::readData(char *data, qint64 maxSize) {
    /*qint64 number = qMin(maxSize, bytesToRead - offset);
    memcpy(data, buffer + offset, number);
    offset += number;

    if (offset >= bytesToRead)
        fillBuffer();

    return number;*/

    buffer2->mutex.lock();
    if (buffer2->nUsedBytes == 0)
        buffer2->notEmpty.wait(&buffer2->mutex);
    qint64 number = qMin(maxSize, buffer2->nUsedBytes);
    buffer2->mutex.unlock();

    if(buffer2->offset + number <= BUFFER_SIZE)
        memcpy(data, buffer2->data+buffer2->offset, number);
    else {
        memcpy(data, buffer2->data+buffer2->offset, BUFFER_SIZE - buffer2->offset);
        memcpy(data+BUFFER_SIZE-buffer2->offset, buffer2->data, number-(BUFFER_SIZE-buffer2->offset));
    }

    buffer2->mutex.lock();
    buffer2->nUsedBytes -= number;
    buffer2->offset =  (buffer2->offset+number) % BUFFER_SIZE;
    //qDebug() << buffer2->offset;
    buffer2->notFull.wakeAll();
    buffer2->mutex.unlock();

    return number;
    return 0;
}

qint64 PirateNetworkReply::bytesAvailable() const {
    //return bytesToRead - offset + QIODevice::bytesAvailable();
    return buffer2->nUsedBytes;
}

void PirateNetworkReply::abort() {
    toAbort = true;
}
