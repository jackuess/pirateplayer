#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include <QWidget>
#include <QProgressBar>
#include <QPushButton>
#include <QLayout>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QFile>

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "downloadhttp.h"
#include "downloadrtmp.h"

class DownloadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DownloadWidget(QWidget *parent = 0, QNetworkAccessManager *qnam = 0);
    ~DownloadWidget();

    void startDownload(QString url, QString subtitlesUrl, QString fileName, QString fetchUrl, bool resume = false);

signals:
    void kill();

private slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void writeToFile();
    void on_killButtonClicked();
    void finished();
    void subtitlesFinished();

private:
    QProgressBar *progressBar;
    QPushButton *killButton;
    QNetworkAccessManager *networkAccessManager;
    QNetworkReply *networkReply;
    QNetworkReply *subtitlesReply;
    Download *downloader;
    QFile file;

    int GetLastKeyframe(FILE * file, int nSkipKeyFrames, uint32_t * dSeek, char **initialFrame, int *initialFrameType, uint32_t * nInitialFrameSize);
};

#endif // DOWNLOADWIDGET_H
