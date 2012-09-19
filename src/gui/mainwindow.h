#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "messagelabel.h"
#include "../models/downloadlistmodel.h"
#include "../models/streamtablemodel.h"
#include "../models/twitterfeedmodel.h"
#include "../network/piratenetworkaccessmanager.h"

#include <QMainWindow>
#include <QLabel>
#include <QItemSelection>
#include <QDataWidgetMapper>
#include <QSettings>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    enum Messages {
        Info = 0,
        Error
    };
    
private:
    void getStreams(QString url);
    void setupTwitter();
    void readUserSettings();
    void writeUserSettings();

    static bool versionNumberGreater(QString leftHand, QString rightHand);

    Ui::MainWindow *ui;
    DownloadListModel *downloadStack;
    PirateNetworkAccessManager *qnam;
    QDataWidgetMapper *twitterWidgetMapper;
    QSettings settings;
    QHash<QString,QVariant> userSettings;
    QHash<QNetworkReply*,MessageLabel*> messages;
    QString applicationVersion;

private slots:
    void getStreamsFinished();
    void adressTabChanged(int index);
    void tabChanged(int index);
    void linkClicked(QUrl url);
    void fetch();
    void enableTwitterButtons(int currentIndex);
    void resetUserSettings();
};

#endif // MAINWINDOW_H
