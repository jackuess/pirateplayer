#include <QDebug>
#include <QTimer>

#include "piratenetworkreply.h"

int
init_sockets()
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

PirateNetworkReply::PirateNetworkReply(QObject *parent, QString rtmpUrl, RtmpResume resumeData) :
    QNetworkReply(parent)
{
    started = false;
    rtmpBufferTime = DEF_BUFTIME;

    init_sockets();

    rtmp = RTMP_Alloc();
    RTMP_Init(rtmp);

    buffer = new ring_buffer;
    buffer->nUsedBytes = 0;
    buffer->offset = 0;

    QByteArray ba = rtmpUrl.toLocal8Bit();
    if (RTMP_SetupURL(rtmp, ba.data())) {
        RTMP_SetBufferMS(rtmp, rtmpBufferTime);
        if(RTMP_Connect(rtmp, NULL)) {
            if(RTMP_ConnectStream(rtmp, resumeData.dSeek)) {
                open(ReadOnly);
                if (resumeData.dSeek > 0) {
                    rtmp->m_read.flags |= RTMP_READ_RESUME;
                    rtmp->m_read.initialFrameType = resumeData.initialFrameType;
                    rtmp->m_read.nResumeTS = resumeData.dSeek;
                    //rtmp->m_read.metaHeader = resumeData.metaHeader;
                    rtmp->m_read.initialFrame = resumeData.initialFrame;
                    //rtmp->m_read.nMetaHeaderSize = resumeData.nMetaHeaderSize;
                    rtmp->m_read.nInitialFrameSize = resumeData.nInitialFrameSize;
                }
                rtmpSession = new RtmpSession(this, rtmp, buffer);
                started = true;
                connect(rtmpSession, SIGNAL(downloadProgress(qint64,qint64)), this, SIGNAL(downloadProgress(qint64,qint64)));
                connect(rtmpSession, SIGNAL(readyRead()), this, SIGNAL(readyRead()));
                connect(rtmpSession, SIGNAL(finished()), this, SIGNAL(finished()));
                rtmpSession->start();
            }
        }
    }
}

PirateNetworkReply::~PirateNetworkReply() {
    if (started) {
        if (rtmpSession->isRunning()) {
            rtmpSession->abort();
            emit readyRead();
            rtmpSession->wait();
        }
        delete rtmpSession;
    }
    delete buffer;
    RTMP_Free(rtmp);
}

bool PirateNetworkReply::isSequential() const {
    return false;
}

qint64 PirateNetworkReply::readData(char *data, qint64 maxSize) {
    buffer->mutex.lock();
    if (buffer->nUsedBytes == 0)
        buffer->notEmpty.wait(&buffer->mutex);
    qint64 number = qMin(maxSize, buffer->nUsedBytes);
    buffer->mutex.unlock();

    if(buffer->offset + number <= BUFFER_SIZE)
        memcpy(data, buffer->data+buffer->offset, number);
    else {
        memcpy(data, buffer->data+buffer->offset, BUFFER_SIZE - buffer->offset);
        memcpy(data+BUFFER_SIZE-buffer->offset, buffer->data, number-(BUFFER_SIZE-buffer->offset));
    }

    buffer->mutex.lock();
    buffer->nUsedBytes -= number;
    buffer->offset =  (buffer->offset+number) % BUFFER_SIZE;
    buffer->notFull.wakeAll();
    buffer->mutex.unlock();

    return number;
}

qint64 PirateNetworkReply::bytesAvailable() const {
    return buffer->nUsedBytes + QIODevice::bytesAvailable();
}

void PirateNetworkReply::abort() {
    if (started && rtmpSession->isRunning()) {
        rtmpSession->abort();
        emit readyRead();
        //read(bytesAvailable());
        rtmpSession->wait();
    }
    emit error();
}
