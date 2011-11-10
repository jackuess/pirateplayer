#ifndef DOWNLOADWIDGET_H
#define DOWNLOADWIDGET_H

#include <QWidget>
#include <QProgressBar>
#include <QPushButton>
#include <QLayout>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QFile>

class DownloadWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DownloadWidget(QWidget *parent = 0, QNetworkAccessManager *qnam = 0);

    void startDownload(QString url, QString fileName, QString fetchUrl);

signals:
    void kill();

private slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void writeToFile();
    void on_killButtonClicked();
    void finished();

private:
    QProgressBar *progressBar;
    QPushButton *killButton;
    QNetworkAccessManager *networkAccessManager;
    QNetworkReply *networkReply;
    QFile file;
};

#endif // DOWNLOADWIDGET_H
