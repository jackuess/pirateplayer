#include <QDebug>
#include <QNetworkRequest>

#include "piratenetworkaccessmanager.h"
#include "piratenetworkreply.h"

PirateNetworkAccessManager::PirateNetworkAccessManager(QObject *parent) :
    QNetworkAccessManager(parent)
{
}

QNetworkReply *PirateNetworkAccessManager::createRequest(Operation op,
                             const QNetworkRequest &request,
                             QIODevice *outgoingData) {
    if(request.url().scheme().startsWith("rtmp"))
        return new PirateNetworkReply(this, request.url().toString(), request.attribute(QNetworkRequest::User).value<RtmpResume>());
    else
        return QNetworkAccessManager::createRequest(op, request, outgoingData);
}
