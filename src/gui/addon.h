#ifndef ADDON_H
#define ADDON_H

#include "mainwindow.h"

#include <QWidget>
#include <QNetworkAccessManager>
#include <QSettings>
#include <QtDeclarative>
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
    void loadingError();

    QNetworkAccessManager *nam;
    QSettings *settings;
    QString name;
    QString dir;
    QByteArray pendingMd5;
    MainWindow *mainWindow;
    QDeclarativeView *declarativeView;

private slots:
    void checkForUpdate();
    void load();
    void upgrade();
    void onViewStatusChange(QDeclarativeView::Status status);
};

class NetworkAccessManagerFactory : public QDeclarativeNetworkAccessManagerFactory
{
    //Q_OBJECT
public:
    QNetworkAccessManager *create(QObject *parent);

};

#endif // ADDON_H
