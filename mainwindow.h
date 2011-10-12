#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <piratenetworkaccessmanager.h>

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

    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void writeToFile();
    void ppDownloadFinished(QNetworkReply * reply);
    void fetchFinished(QNetworkReply * reply);

private:
    Ui::MainWindow *ui;
    PirateNetworkAccessManager *networkAccessManager;
    QNetworkReply *networkReply;
    QFile file;
};

#endif // MAINWINDOW_H
