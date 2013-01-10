#include "rtmpsession.h"

#include <QDebug>
#include <QTimer>

#include <stdio.h>

RtmpSession::RtmpSession(QObject *parent, RTMP *r, ring_buffer *buff) :
    QThread(parent)
{
    toAbort = false;
    rtmp = r;
    buffer = buff;
    buffer->data = new char[BUFFER_SIZE];
}

RtmpSession::~RtmpSession() {
    toAbort = true;
    delete[] buffer->data;
}

void RtmpSession::run() {
    int bufferSize = 0;
    size = 0;
    qint64 nRead = 0;
    double duration = 0.0;
    char *readBuffer;
    uint32_t rtmpBufferTime = DEF_BUFTIME;

    do {
        buffer->mutex.lock();
        if (buffer->nUsedBytes == BUFFER_SIZE)
            buffer->notFull.wait(&buffer->mutex);
        bufferSize = BUFFER_SIZE - buffer->nUsedBytes;
        buffer->mutex.unlock();

        readBuffer = new char[bufferSize];
        nRead = RTMP_Read(rtmp, readBuffer, bufferSize);
        size += nRead;

        int beginFreeBytes = (buffer->offset + buffer->nUsedBytes) % BUFFER_SIZE;
        int sizeFirstChunk = BUFFER_SIZE - beginFreeBytes;
        if(nRead <= sizeFirstChunk)
            memcpy(buffer->data + beginFreeBytes, readBuffer, nRead);
        else {
            memcpy(buffer->data + beginFreeBytes, readBuffer, sizeFirstChunk);
            memcpy(buffer->data, readBuffer + sizeFirstChunk, nRead - sizeFirstChunk);
        }

        buffer->mutex.lock();
        buffer->nUsedBytes += nRead;
        buffer->notEmpty.wakeAll();
        buffer->mutex.unlock();

        delete[] readBuffer;

        duration = RTMP_GetDuration(rtmp);

        // Make sure we claim to have enough buffer time!
        if (rtmpBufferTime < (duration * 1000.0)) {
            rtmpBufferTime = (uint32_t) (duration * 1000.0) + 5000;   // extra 5sec to make sure we've got enough
            RTMP_SetBufferMS(rtmp, rtmpBufferTime);
            RTMP_UpdateBufferMS(rtmp);
        }

        if (rtmp->m_read.timestamp > 0 && duration > 0)
            emit downloadProgress(size, 1000*duration * (size / rtmp->m_read.timestamp));
        //QTimer::singleShot( 0, this, SIGNAL(readyRead()) );
        emit readyRead();

        if (RTMP_IsTimedout(rtmp)) {
            qDebug() << "Trying to reconnect";
            RTMP_ToggleStream(rtmp);
        }
    } while (!toAbort && nRead > -1 && RTMP_IsConnected(rtmp));

    if (!toAbort)
        emit finished();
}

void RtmpSession::abort() {
    toAbort = true;
}
