#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void pushMessage(QString msg, int type = 0);
    void getStreams(QString url);
    void setupTwitter();
    void readUserSettings();
    void writeUserSettings();

    Ui::MainWindow *ui;
    DownloadListModel *downloadStack;
    PirateNetworkAccessManager *qnam;
    QNetworkReply *getStreamsReply;
    QDataWidgetMapper *twitterWidgetMapper;
    QSettings settings;
    QHash<QString,QVariant> userSettings;

private slots:
    void getStreamsFinished();
    void deleteLastMessage();
    void adressTabChanged(int index);
    void tabChanged(int index);
    void linkClicked(QUrl url);
    void fetchClicked();
    void enableTwitterButtons(int currentIndex);
    void openCurrentTweet();
    void resetUserSettings();
};

#endif // MAINWINDOW_H
