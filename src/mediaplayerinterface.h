#ifndef MEDIAPLAYERINTERFACE_H
#define MEDIAPLAYERINTERFACE_H

#include <QObject>
#include <QNetworkAccessManager>

class MediaPlayerInterface : public QObject
{
    Q_OBJECT
public:
    explicit MediaPlayerInterface(QObject *parent = 0);

    void setNam(QNetworkAccessManager *newNam);
    void setTempDir(const QString &newTempDir);
    void play(const QString &url, const QString &subsUrl = "", const QString defaultCmdTemplate = "");
    
signals:
    
public slots:

private:
    void playNative();
    void playUsingCmd(const QString &cmdTemplate);
    void playTemp();

    QNetworkAccessManager *nam;
    QString tempDir;
    QString _url;
    QString _subsUrl;
    
};

#endif // MEDIAPLAYERINTERFACE_H
