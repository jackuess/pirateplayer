#ifndef PIRATENETWORKACCESSMANAGER_H
#define PIRATENETWORKACCESSMANAGER_H

#include <QNetworkAccessManager>

class PirateNetworkAccessManager : public QNetworkAccessManager
{
    Q_OBJECT
public:
    explicit PirateNetworkAccessManager(QObject *parent = 0);

protected:
    QNetworkReply *createRequest(Operation op, const QNetworkRequest &request, QIODevice *outgoingData);
};

#endif // PIRATENETWORKACCESSMANAGER_H
