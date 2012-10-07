#include <QDebug>
#include <QNetworkRequest>

#include "piratenetworkaccessmanager.h"
#include "piratenetworkreply.h"
#include "tidynetworkreply.h"

PirateNetworkAccessManager::PirateNetworkAccessManager(QObject *parent) :
    QNetworkAccessManager(parent)
{
}

QNetworkReply *PirateNetworkAccessManager::createRequest(Operation op,
                             const QNetworkRequest &request,
                             QIODevice *outgoingData) {
    QString protocol = request.url().scheme();
    if(protocol.startsWith("rtmp"))
        return new PirateNetworkReply(this, request.url().toString(), request.attribute(QNetworkRequest::User).value<RtmpResume>());
    else if(op == QNetworkAccessManager::GetOperation && protocol == "tidy") {
        QNetworkRequest newReq(request);
        newReq.url().setScheme("http://");
        QUrl newUrl = newReq.url();
        newUrl.setScheme("http");
        newReq.setUrl(newUrl);
        return new TidyNetworkReply(newReq, this);
    }
    else
        return QNetworkAccessManager::createRequest(op, request, outgoingData);
}
