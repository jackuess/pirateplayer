#ifndef PIRATEPLAYERDESKTOPVIEWER_H
#define PIRATEPLAYERDESKTOPVIEWER_H

#include <QObject>
#include <QDeclarativeEngine>
#include <QDeclarativeComponent>
#include <QGraphicsObject>
#include <QSettings>

#include "abstractqmlviewer.h"
#include "../pirateplayercontext.h"

class PirateplayerDesktopViewer : public QObject, public AbstractQmlViewer
{
    Q_OBJECT
public:
    explicit PirateplayerDesktopViewer(QObject *parent = 0);
    ~PirateplayerDesktopViewer() { ; }

    void addLibraryPath(const QString& lib);
    void addPluginPath(const QString& plugin);

    QString currentFile() const { return currentFileOrUrl; }
    QDeclarativeEngine *engine() { return _engine; }
    QGraphicsObject *rootObject() { return _rootObject; }
    QDeclarativeContext *rootContext() { return _engine->rootContext(); }
    
signals:
    void statusChanged(QDeclarativeComponent::Status);
    
public slots:
    bool open(const QString&);
    void statusChanged() {}
    void execute(QUrl url);
    void execute2();
    void quit();

private:
    QString currentFileOrUrl;
    QDeclarativeEngine *_engine;
    QGraphicsObject *_rootObject;
    QDeclarativeComponent *_component;

    void onComponentError();
    
};

#endif // PIRATEPLAYERDESKTOPVIEWER_H
