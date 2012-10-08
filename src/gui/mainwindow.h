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
    static bool versionNumberGreater(QString leftHand, QString rightHand);

    enum Messages {
        Info = 0,
        Error
    };

public slots:
    void getStreams(QString url, QVariantMap metaData = QVariantMap());

private:
    void setupTwitter();
    void readUserSettings();
    void writeUserSettings();

    Ui::MainWindow *ui;
    DownloadListModel *downloadStack;
    PirateNetworkAccessManager *qnam;
    QDataWidgetMapper *twitterWidgetMapper;
    QSettings settings;
    QHash<QString,QVariant> userSettings;
    QHash<QObject*,MessageLabel*> messages;

private slots:
    void streamsFound();
    void noStreamsFound();
    void tabChanged(int index);
    void linkClicked(QUrl url);
    void fetch();
    void enableTwitterButtons(int currentIndex);
    void resetUserSettings();
    void setAddonTitle(QString newTitle);
    void setupAddons();
};

#endif // MAINWINDOW_H
