#ifndef MAINGUI_H
#define MAINGUI_H

#include "abstractqmlviewer.h"
#include "../extra/archiveextractor.h"

#include <QObject>
#include <QNetworkAccessManager>

class MainGui : public QObject
{
    Q_OBJECT
public:
    explicit MainGui(QObject *parent = 0);
    ~MainGui();

    void show();
    
signals:
    
public slots:

private:
    AbstractQmlViewer *qmlViewer;
    QNetworkAccessManager *nam;
    ArchiveExtractor *extractor;
    static bool removeDir(const QString &dirName);
    QString pkgDir;
    QString pendingMd5Sum;
    QSettings settings;
    QString pirateplaySeBase;

    void setFont();

private slots:
    void findPkg();
    void extractPkg();
    void load();
    
};

#endif // MAINGUI_H
