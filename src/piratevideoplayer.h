#ifndef PIRATEVIDEOPLAYER_H
#define PIRATEVIDEOPLAYER_H

#include <phonon/mediaobject.h>
#include <phonon/audiooutput.h>
#include <phonon/videowidget.h>
#include <phonon/SeekSlider>
#include <phonon/VolumeSlider>

#include <QWidget>
#include <QtGui/QApplication>
#include <QtGui/QMainWindow>
#include <QProgressBar>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QDebug>
#include <QLayout>
#include <QTemporaryFile>
#include <QTimer>
#include <QMargins>
#include <QMenu>
#include <QPushButton>
#include <QLabel>
#include <QIcon>

class PirateVideoPlayer : public QWidget
{
    Q_OBJECT
public:
    explicit PirateVideoPlayer(QWidget *parent = 0, QNetworkAccessManager *qnam = 0);

    void playUrl(QString url);

private:
    //Widgets
    Phonon::VideoWidget *videoWidget;
    Phonon::SeekSlider *slider;
    Phonon::VolumeSlider *volumeSlider;
    QMenu contextMenu;
    QWidget *controlPanel;
    QPushButton *pbPause;
    QPushButton *pbFullScreen;
    QPushButton *pbVolumeControl;
    QProgressBar *progressBar;
    QLabel *labelCurrentTime;
    QLabel *labelTotalTime;

    QTemporaryFile tmpFile;
    QNetworkAccessManager *networkAccess;
    QMargins defaultContentsMargins;
    Phonon::State state;
    QTimer *timerHideInterface;
    Phonon::MediaObject* media;
    Phonon::AudioOutput *audioOutput;
    QNetworkReply *reply;

    QString formatMs(qint64 ms);

    void mouseMoveEvent(QMouseEvent *event);

private slots:
    void read();
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void createContextMenu(QPoint pos);
    void toggleFullScreen(bool checked);
    void togglePlay(bool checked);
    void toggleVolumeControll(bool checked);
    void hideInterface();
    void updateTotalTime(qint64 newTotalTime);
    void updateCurrentTime(qint64 time);
};

#endif // PIRATEVIDEOPLAYER_H
