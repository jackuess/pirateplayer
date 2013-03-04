#ifndef ABSTRACTQMLVIEWER_H
#define ABSTRACTQMLVIEWER_H

#include <QDeclarativeNetworkAccessManagerFactory>

#include "../network/piratenetworkaccessmanager.h"
#include "../pirateplayercontext.h"

class NetworkAccessManagerFactory : public QDeclarativeNetworkAccessManagerFactory
{
    //Q_OBJECT
public:
    QNetworkAccessManager *create(QObject *parent) { return new PirateNetworkAccessManager(parent); }

};

class AbstractQmlViewer {
public:
    virtual bool open(const QString &url) = 0;
    virtual void addLibraryPath(const QString& lib) = 0;
    virtual ~AbstractQmlViewer() { ; }

    PirateplayerContext* getPpContext() { return &ppContext; }

protected:
    PirateplayerContext ppContext;
    NetworkAccessManagerFactory namFactory;
};

#endif // ABSTRACTQMLVIEWER_H
