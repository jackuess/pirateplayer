#ifndef ADDON_H
#define ADDON_H


#include "windows.h"
#include "winbase.h"

#include "mainwindow.h"

#include <QWidget>
#include <QNetworkAccessManager>
#include <QSettings>
#include <QDeclarativeNetworkAccessManagerFactory>

class NetworkAccessManagerFactory;

class Addon : public QWidget
{
    Q_OBJECT
public:
    explicit Addon(const QString &name, QNetworkAccessManager *nam, QSettings *settingsObj, MainWindow *mainWindow, QWidget *parent = 0);

signals:
    void titleSet(QString newTitle);
    
private:
    void download();
    static bool removeDir(const QString &dirName);

    QNetworkAccessManager *nam;
    QSettings *settings;
    QString name;
    QString dir;
    QByteArray pendingMd5;
    MainWindow *mainWindow;

private slots:
    void checkForUpdate();
    void load();
    void upgrade();
};

class NetworkAccessManagerFactory : public QDeclarativeNetworkAccessManagerFactory
{
    //Q_OBJECT
public:
    QNetworkAccessManager *create(QObject *parent);

};

#endif // ADDON_H
