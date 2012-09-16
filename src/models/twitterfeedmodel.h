#ifndef TWITTERFEEDMODEL_H
#define TWITTERFEEDMODEL_H

#include <QAbstractTableModel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QList>
#include <QHash>

class TwitterFeedModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TwitterFeedModel(QNetworkAccessManager *nam, QObject *parent = 0);

    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

    void setScreenName(QString name);

    enum Columns {
        TextColumn = 0,
        PubDateColumn,
        LinkColumn,
        COLUMN_COUNT
    };
    
signals:
    
private slots:
    void feedDownloaded();

private:
    QList<QHash<QString,QString> > tweets;
    QNetworkAccessManager *network;
    QNetworkReply *feed;
    QString screenName;
};

#endif // TWITTERFEEDMODEL_H
