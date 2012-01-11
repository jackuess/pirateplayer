#include "downloadwidget.h"

#include <QDesktopServices>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>
#include <QVariant>

#include "piratenetworkreply.h"

unsigned int
AMF_DecodeInt24(const char *data)
{
  unsigned char *c = (unsigned char *) data;
  unsigned int val;
  val = (c[0] << 16) | (c[1] << 8) | c[2];
  return val;
}

unsigned int
AMF_DecodeInt32(const char *data)
{
  unsigned char *c = (unsigned char *)data;
  unsigned int val;
  val = (c[0] << 24) | (c[1] << 16) | (c[2] << 8) | c[3];
  return val;
}


DownloadWidget::DownloadWidget(QWidget *parent, QNetworkAccessManager *qnam) :
    QWidget(parent)
{
    networkAccessManager = qnam;
}

DownloadWidget::~DownloadWidget() {
    //delete downloader;
    if (file.isOpen()) {
        file.close();
    }
}

void DownloadWidget::startDownload(QString url, QString subtitlesUrl, QString fileName, QString fetchUrl) {
    resumeSeek = 0;
    QNetworkRequest req;
    QGridLayout *layout = new QGridLayout(this);
    WriteMode mode = OverWrite;
    RtmpResume resumeData = RtmpResume();

    progressBar = new QProgressBar(this);
    killButton = new QPushButton("Avbryt", this);
    QLabel *label = new QLabel(fetchUrl, this);
    layout->addWidget(label, 0, 0, 1, 2);
    layout->addWidget(progressBar, 1, 0);
    layout->addWidget(killButton, 1, 1);
    connect(killButton, SIGNAL(clicked()), this, SLOT(on_killButtonClicked()));
    setLayout(layout);

    FILE * pFile;

    file.setFileName(fileName);
    if (file.exists())
        mode = queryFileExists();

    switch (mode) {
        case Resume:
            if (url.startsWith("rtmp")) {
                QVariant vResume;
                QByteArray ba = fileName.toLocal8Bit();
                pFile = fopen(ba.data(), "r+b");
                if (pFile!=NULL) {
                    if (!GetLastKeyframe(pFile, 0, &resumeData.dSeek, &resumeData.initialFrame, &resumeData.initialFrameType, &resumeData.nInitialFrameSize) == 0)
                        resumeData.dSeek = 0;
                    qDebug() << "Seek: " << resumeData.dSeek;
                    uint32_t seek = ftell(pFile);
                    fclose (pFile);
                    //free(initialFrame);
                    if (resumeData.dSeek > 0) {
                        file.open(QIODevice::ReadWrite);
                        file.seek(seek);
                    }
                    else
                        file.open(QIODevice::WriteOnly);
                }
                vResume.setValue(resumeData);
                req.setAttribute(QNetworkRequest::User, vResume);
            }
            else {
                resumeSeek = file.size();
                file.open(QIODevice::WriteOnly | QIODevice::Append);
                req.setRawHeader("Range", "bytes=" + QByteArray::number(resumeSeek) + "-");
            }
            break;

        case OverWrite:
            file.open(QIODevice::WriteOnly);
            break;

        case Abort:
            emit kill();
            return;
    }

    req.setUrl(QUrl(url));
    networkReply = networkAccessManager->get(req);
    connect(networkReply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));
    connect(networkReply, SIGNAL(readyRead()), this, SLOT(writeToFile()));
    connect(networkReply, SIGNAL(finished()), this, SLOT(replyFinished()));
    connect(networkReply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(replyError(QNetworkReply::NetworkError)));

    if (subtitlesUrl != "") {
        //Catch xml and tt extension
        QString extension = subtitlesUrl.mid(subtitlesUrl.lastIndexOf('.'));
        if (extension == ".xml" || extension == ".tt")
            subtitlesUrl = "http://pirateplay.se/convertsub;default.srt?url=" + subtitlesUrl;
        subtitlesReply = networkAccessManager->get(QNetworkRequest(QUrl(subtitlesUrl)));
        connect(subtitlesReply, SIGNAL(finished()), this, SLOT(subtitlesFinished()));
    }
}

void DownloadWidget::downloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    progressBar->setValue((resumeSeek+(int)((double)bytesReceived))/(double)bytesTotal*100.0);
}

void DownloadWidget::writeToFile() {
    file.write(networkReply->read(networkReply->bytesAvailable()));
}

void DownloadWidget::on_killButtonClicked() {
    if(file.isOpen()) {
        networkReply->abort();
        delete networkReply;
    }
    emit kill();
}

void DownloadWidget::replyFinished() {
    networkReply->deleteLater();
    file.close();
    progressBar->setValue(100);
    killButton->setText("Stäng");
}

void DownloadWidget::replyError(QNetworkReply::NetworkError e) {
    networkReply->deleteLater();
    killButton->setText("Stäng");
}

void DownloadWidget::subtitlesFinished() {
    if (subtitlesReply->error() == QNetworkReply::NoError && subtitlesReply->size() > 0) {
        QFile subsFile;
        QString fileName = file.fileName().left(file.fileName().lastIndexOf('.'));
        QString reqUrl = subtitlesReply->request().url().toString();

        //Add extension
        fileName += reqUrl.remove(0, reqUrl.lastIndexOf('.')).replace(QRegExp("(wsrt)|(xml)|(tt)$"), "srt").replace(QRegExp("smi\\?$"), ".smi");

        subsFile.setFileName(fileName);
        subsFile.open(QIODevice::WriteOnly);
        subsFile.write(subtitlesReply->readAll());
        subtitlesReply->deleteLater();

        layout()->addWidget(new QLabel("Undertext sparad till " + fileName));
    }
}

DownloadWidget::WriteMode DownloadWidget::queryFileExists() {
    QMessageBox msgBox;
    QPushButton *bOverwrite = msgBox.addButton("Skriv över filen", QMessageBox::ActionRole);
    QPushButton *bResume = msgBox.addButton("Försök återuppta avbruten nedladdning", QMessageBox::ActionRole);
    QPushButton *bAbort = msgBox.addButton(QMessageBox::Abort);
    WriteMode r;

    msgBox.setText("Filen existerar redan. Vad vill du göra?");
    msgBox.exec();
    if (msgBox.clickedButton() == bOverwrite)
        r = OverWrite;
    else if (msgBox.clickedButton() == bResume)
        r = Resume;
    else
        r = Abort;

    delete bOverwrite;
    delete bResume;
    delete bAbort;

    return r;
}

int DownloadWidget::GetLastKeyframe(FILE *file, int nSkipKeyFrames, uint32_t *dSeek, char **initialFrame, int *initialFrameType, uint32_t *nInitialFrameSize) {
    const size_t bufferSize = 16;
      char buffer[bufferSize];
      uint8_t dataType;
      int bAudioOnly;
      off_t size;

      fseek(file, 0, SEEK_END);
      size = ftello(file);

      fseek(file, 4, SEEK_SET);
      if (fread(&dataType, sizeof(uint8_t), 1, file) != 1)
        return -1;

      bAudioOnly = (dataType & 0x4) && !(dataType & 0x1);

      printf("bAudioOnly: %d, size: %llu\n", bAudioOnly,
          (unsigned long long) size);

      // ok, we have to get the timestamp of the last keyframe (only keyframes are seekable) / last audio frame (audio only streams)

      //if(!bAudioOnly) // we have to handle video/video+audio different since we have non-seekable frames
      //{
      // find the last seekable frame
      off_t tsize = 0;
      uint32_t prevTagSize = 0;

      // go through the file and find the last video keyframe
      do
        {
          int xread;
        skipkeyframe:
          if (size - tsize < 13)
        {
          printf("Unexpected start of file, error in tag sizes, couldn't arrive at prevTagSize=0");
          return -1;
        }
          fseeko(file, size - tsize - 4, SEEK_SET);
          xread = fread(buffer, 1, 4, file);
          if (xread != 4)
        {
          printf("Couldn't read prevTagSize from file!");
          return -1;
        }

          prevTagSize = AMF_DecodeInt32(buffer);
          //printf(printfDEBUG, "Last packet: prevTagSize: %d", prevTagSize);

          if (prevTagSize == 0)
        {
          printf("Couldn't find keyframe to resume from!");
          return -1;
        }

          if (prevTagSize < 0 || prevTagSize > size - 4 - 13)
        {
          printf("Last tag size must be greater/equal zero (prevTagSize=%d) and smaller then filesize, corrupt file!\n",
              prevTagSize);
          return -1;
        }
          tsize += prevTagSize + 4;

          // read header
          fseeko(file, size - tsize, SEEK_SET);
          if (fread(buffer, 1, 12, file) != 12)
        {
          printf("Couldn't read header!");
          return -1;
        }
          //*
    #ifdef _DEBUG
          uint32_t ts = AMF_DecodeInt24(buffer + 4);
          ts |= (buffer[7] << 24);
          printf("%02X: TS: %d ms", buffer[0], ts);
    #endif //*/

          // this just continues the loop whenever the number of skipped frames is > 0,
          // so we look for the next keyframe to continue with
          //
          // this helps if resuming from the last keyframe fails and one doesn't want to start
          // the download from the beginning
          //
          if (nSkipKeyFrames > 0
          && !(!bAudioOnly
               && (buffer[0] != 0x09 || (buffer[11] & 0xf0) != 0x10)))
        {
    #ifdef _DEBUG
          printf("xxxxxxxxxxxxxxxxxxxxxxxx Well, lets go one more back!");
    #endif
          nSkipKeyFrames--;
          goto skipkeyframe;
        }

        }
      while ((bAudioOnly && buffer[0] != 0x08) || (!bAudioOnly && (buffer[0] != 0x09 || (buffer[11] & 0xf0) != 0x10)));	// as long as we don't have a keyframe / last audio frame

      // save keyframe to compare/find position in stream
      *initialFrameType = buffer[0];
      *nInitialFrameSize = prevTagSize - 11;
      *initialFrame = (char *) malloc(*nInitialFrameSize);

      fseeko(file, size - tsize + 11, SEEK_SET);
      if (fread(*initialFrame, 1, *nInitialFrameSize, file) != *nInitialFrameSize)
        {
          printf("Couldn't read last keyframe, aborting!");
          return -1;
        }

      *dSeek = AMF_DecodeInt24(buffer + 4);	// set seek position to keyframe tmestamp
      *dSeek |= (buffer[7] << 24);
      //}
      //else // handle audio only, we can seek anywhere we'd like
      //{
      //}

      if (*dSeek < 0)
        {
          printf("Last keyframe timestamp is negative, aborting, your file is corrupt!");
          return -1;
        }
      printf("Last keyframe found at: %d ms, size: %d, type: %02X\n", *dSeek,
          *nInitialFrameSize, *initialFrameType);

      /*
         // now read the timestamp of the frame before the seekable keyframe:
         fseeko(file, size-tsize-4, SEEK_SET);
         if(fread(buffer, 1, 4, file) != 4) {
         printf("Couldn't read prevTagSize from file!");
         goto start;
         }
         uint32_t prevTagSize = RTMP_LIB::AMF_DecodeInt32(buffer);
         fseeko(file, size-tsize-4-prevTagSize+4, SEEK_SET);
         if(fread(buffer, 1, 4, file) != 4) {
         printf("Couldn't read previous timestamp!");
         goto start;
         }
         uint32_t timestamp = RTMP_LIB::AMF_DecodeInt24(buffer);
         timestamp |= (buffer[3]<<24);

         printf(printfDEBUG, "Previous timestamp: %d ms", timestamp);
       */

      if (*dSeek != 0)
        {
          // seek to position after keyframe in our file (we will ignore the keyframes resent by the server
          // since they are sent a couple of times and handling this would be a mess)
          fseeko(file, size - tsize + prevTagSize + 4, SEEK_SET);

          // make sure the WriteStream doesn't write headers and ignores all the 0ms TS packets
          // (including several meta data headers and the keyframe we seeked to)
          //bNoHeader = TRUE; if bResume==true this is true anyway
        }

      //}

      return 0;
}

