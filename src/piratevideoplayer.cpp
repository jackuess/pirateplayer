#include "piratevideoplayer.h"


PirateVideoPlayer::PirateVideoPlayer(QWidget *parent, QNetworkAccessManager *qnam) :
    QWidget(parent)
{
    state = Phonon::StoppedState;
    networkAccess = qnam;

    //Set up UI
    controlPanel = new QWidget(this);
    pbPause = new QPushButton(QIcon("/home/chucky/utveckling/phonon_test/High-contrast-media-playback-pause.svg"), "", controlPanel);
    pbFullScreen = new QPushButton(QIcon("/home/chucky/utveckling/phonon_test/High-contrast-view-fullscreen.svg"), "", controlPanel);
    pbVolumeControl = new QPushButton(QIcon("/home/chucky/utveckling/phonon_test/High-contrast-multimedia-volume-control.svg"), "", controlPanel);
    pbPause->setCheckable(true);
    pbFullScreen->setCheckable(true);
    pbVolumeControl->setCheckable(true);
    labelCurrentTime = new QLabel(controlPanel);
    labelTotalTime = new QLabel(controlPanel);
    videoWidget = new Phonon::VideoWidget(this);
    volumeSlider = new Phonon::VolumeSlider(videoWidget);
    volumeSlider->setAutoFillBackground(true);
    volumeSlider->setOrientation(Qt::Vertical);
    volumeSlider->hide();
    progressBar = new QProgressBar(controlPanel);
    progressBar->setFormat("");
    QVBoxLayout *progressLayout = new QVBoxLayout();
    progressLayout->setContentsMargins(0, 0, 0, 0);
    progressBar->setLayout(progressLayout);
    slider = new Phonon::SeekSlider(progressBar);
    slider->setIconVisible(false);
    progressBar->layout()->addWidget(slider);
    controlPanel->setLayout(new QHBoxLayout());
    controlPanel->layout()->addWidget(pbPause);
    controlPanel->layout()->addWidget(labelCurrentTime);
    controlPanel->layout()->addWidget(progressBar);
    controlPanel->layout()->addWidget(labelTotalTime);
    controlPanel->layout()->addWidget(pbVolumeControl);
    controlPanel->layout()->addWidget(pbFullScreen);
    controlPanel->setEnabled(false);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(videoWidget, 2);
    layout->addWidget(controlPanel, 0);
    setLayout(layout);
    defaultContentsMargins = layout->contentsMargins();
    videoWidget->hide();

    //Set up Phonon
    media = new Phonon::MediaObject(this);
    audioOutput = new Phonon::AudioOutput(Phonon::MusicCategory, this);
    media->setTickInterval(500);
    connect(media, SIGNAL(totalTimeChanged(qint64)), this, SLOT(updateTotalTime(qint64)));
    connect(media, SIGNAL(tick(qint64)), this, SLOT(updateCurrentTime(qint64)));
    Phonon::createPath(media, audioOutput);
    Phonon::createPath(media, videoWidget);
    slider->setMediaObject(media);
    volumeSlider->setAudioOutput(audioOutput);

    //Actions
    videoWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(videoWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(createContextMenu(QPoint)));
    QAction *actionFullScreen = new QAction("Fullskärm", this);
    QAction *actionPlayPause = new QAction("Spela/paus", this);
    actionFullScreen->setCheckable(true);
    actionPlayPause->setCheckable(true);
    actionFullScreen->setShortcut(QKeySequence(Qt::Key_F));
    actionPlayPause->setShortcut(QKeySequence(Qt::Key_Space));
    connect(actionFullScreen, SIGNAL(triggered(bool)), this, SLOT(toggleFullScreen(bool)));
    connect(actionPlayPause, SIGNAL(triggered(bool)), this, SLOT(togglePlay(bool)));
    connect(pbPause, SIGNAL(clicked(bool)), actionPlayPause, SLOT(trigger()));
    connect(pbFullScreen, SIGNAL(clicked(bool)), actionFullScreen, SLOT(trigger()));
    connect(pbVolumeControl, SIGNAL(toggled(bool)), this, SLOT(toggleVolumeControll(bool)));
    contextMenu.addAction(actionFullScreen);
    contextMenu.addAction(actionPlayPause);
    videoWidget->addAction(actionFullScreen);
    videoWidget->addAction(actionPlayPause);

    //Timer for hiding UI during playback
    timerHideInterface = new QTimer(this);
    timerHideInterface->setSingleShot(true);
    connect(timerHideInterface, SIGNAL(timeout()), this, SLOT(hideInterface()));
    timerHideInterface->start(3000);
    setMouseTracking(true);
}

void PirateVideoPlayer::playUrl(QString url) {
    reply = networkAccess->get(QNetworkRequest(QUrl(url)));

    state = Phonon::LoadingState;

    connect(reply, SIGNAL(readyRead()), this, SLOT(read()));
    connect(reply, SIGNAL(downloadProgress(qint64,qint64)), this, SLOT(downloadProgress(qint64,qint64)));

    if(!tmpFile.open())
        qDebug() << "Kunde inte öppna den temporära filen.";
}

void PirateVideoPlayer::mouseMoveEvent(QMouseEvent *event) {
    if (controlPanel->isHidden()) {
        controlPanel->show();
        videoWidget->unsetCursor();
        timerHideInterface->stop();
    }
    else {
        timerHideInterface->stop();
        timerHideInterface->start(3000);
    }
}

void PirateVideoPlayer::read() {
    tmpFile.write(reply->read(reply->bytesAvailable()));
}

void PirateVideoPlayer::downloadProgress(qint64 bytesReceived, qint64 bytesTotal) {
    progressBar->setValue((int)((double)bytesReceived/(double)bytesTotal*100.0));

    switch (state) {
    case Phonon::LoadingState:
        if(bytesReceived > 1024 * 1024 * 2) {
            videoWidget->show();
            media->setCurrentSource(tmpFile.fileName());
            media->play();
            state = Phonon::PlayingState;
            controlPanel->setEnabled(true);
        }
        break;
    default:
        break;
    }
}

QString PirateVideoPlayer::formatMs(qint64 ms) {
    int seconds = ms / 1000;
    int hours = (int)(seconds/3600);
    seconds -= hours * 3600;
    int minutes = (int)(seconds/60);
    seconds -= minutes * 60;
    return QString("%1:%2:%3").arg(hours).arg(minutes, 2, 10, QLatin1Char('0')).arg(seconds, 2, 10, QLatin1Char('0'));
}

void PirateVideoPlayer::updateTotalTime(qint64 newTotalTime) {
    labelTotalTime->setText(formatMs(newTotalTime));
}

void PirateVideoPlayer::updateCurrentTime(qint64 time) {
    labelCurrentTime->setText(formatMs(time));
}

void PirateVideoPlayer::createContextMenu(QPoint pos) {
    contextMenu.exec(videoWidget->mapToGlobal(pos));
}

void PirateVideoPlayer::toggleFullScreen(bool checked) {
    if (checked) {
        setWindowFlags(Qt::Window);
        layout()->setContentsMargins(0, 0, 0, 0);
        showFullScreen();
    }
    else {
        setWindowFlags(Qt::Widget);
        layout()->setContentsMargins(defaultContentsMargins);
        showNormal();
    }
    pbFullScreen->setChecked(checked);
}

void PirateVideoPlayer::togglePlay(bool checked) {
    if (checked)
        media->pause();
    else
        media->play();
    pbPause->setChecked(checked);
}

void PirateVideoPlayer::toggleVolumeControll(bool checked) {
    if (checked) {
        volumeSlider->setGeometry(pbVolumeControl->x(), volumeSlider->parentWidget()->height() - 110, pbVolumeControl->width(), 110);
        volumeSlider->show();
    }
    else
        volumeSlider->hide();
}

void PirateVideoPlayer::hideInterface() {
    if (videoWidget->underMouse() && !pbVolumeControl->isChecked()) {
        controlPanel->hide();
        videoWidget->setCursor(Qt::BlankCursor);
    }
}
