#ifndef TIDYNETWORKREPLY_H
#define TIDYNETWORKREPLY_H

#include <tidy.h>
#include <buffio.h>

#include <QNetworkReply>

class TidyNetworkReply : public QNetworkReply
{
    Q_OBJECT
public:
    explicit TidyNetworkReply(const QNetworkRequest &req, QObject *parent = 0);
    ~TidyNetworkReply();
    qint64 bytesAvailable() const;
    bool isSequential() const;
    void abort();

protected:
    qint64 readData(char *data, qint64 maxSize);
    
private:
    QNetworkAccessManager *nam;
    QNetworkReply *reply;

    qint64 offset;

    TidyDoc tdoc;
    TidyBuffer output;
    TidyBuffer errbuf;
    
private slots:
    void tidyUp();
};

#endif // TIDYNETWORKREPLY_H
