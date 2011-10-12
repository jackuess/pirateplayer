#include <QDebug>
#include <QNetworkRequest>

#include "piratenetworkaccessmanager.h"
#include <piratenetworkreply.h>

PirateNetworkAccessManager::PirateNetworkAccessManager(QObject *parent) :
    QNetworkAccessManager(parent)
{
}

QNetworkReply *PirateNetworkAccessManager::createRequest(Operation op,
                             const QNetworkRequest &request,
                             QIODevice *outgoingData) {
    QString protocol = request.url().scheme();
    protocol.truncate(4);
    if(protocol == "rtmp") {
        PirateNetworkReply *reply = new PirateNetworkReply(this, request.url().toString());
        return reply;
    }
    else
        return QNetworkAccessManager::createRequest(op, request, outgoingData);
}
