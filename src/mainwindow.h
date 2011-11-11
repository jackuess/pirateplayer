#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include "piratenetworkaccessmanager.h"
#include "downloadwidget.h"
#include "piratevideoplayer.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_Download_clicked();
    void on_pushButton_Fetch_clicked();
    void on_pbPlay_clicked();

    void fetchFinished(QNetworkReply * reply);
    void killDownloadWidget();

private:
    Ui::MainWindow *ui;
    PirateNetworkAccessManager *networkAccessManager;
    PirateVideoPlayer *videoPlayer;
};

#endif // MAINWINDOW_H
