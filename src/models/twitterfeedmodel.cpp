#include "twitterfeedmodel.h"

#include <QUrl>
#include <QDomDocument>
#include <QNetworkReply>
#include <QtAlgorithms>
#include <QDebug>
#include <QRegExp>
#include <QLocale>

TwitterFeedModel::TwitterFeedModel(QNetworkAccessManager *nam, QObject *parent) :
    QAbstractTableModel(parent)
{
    network = nam;
}

int TwitterFeedModel::rowCount(const QModelIndex &parent) const {
    return tweets.count();
}

int TwitterFeedModel::columnCount(const QModelIndex &parent) const {
    return COLUMN_COUNT;
}

QVariant TwitterFeedModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case TextColumn:
            return QVariant(tweets[index.row()].value("text", ""));
        case LinkColumn:
            return QVariant(tweets[index.row()].value("link", ""));
        case PubDateColumn:
            return QVariant(tweets[index.row()].value("pubdate", ""));
        }
    }

    return QVariant();
}

void TwitterFeedModel::setScreenName(QString name) {
    QUrl url;
    QNetworkRequest req;

    this->screenName = name;

    url.setUrl("http://api.twitter.com/1/statuses/user_timeline.rss");
    url.addQueryItem("screen_name", this->screenName);
    url.addQueryItem("count", "200");

    req.setUrl(url);

    feed = network->get(req);
    connect(feed, SIGNAL(finished()), this, SLOT(feedDownloaded()));
}

QString TwitterFeedModel::relativeTime(const QDateTime &term1, const QDateTime &term2) {
    int dayDiff = term1.daysTo(term2);

    if (dayDiff == 0) {
        int secDiff = -term1.secsTo(term2);
        const int MINUTE = 60;
        const int HOUR = 3600;
        if (secDiff < MINUTE)
            return QString::fromUtf8("idag för %0 %1 sedan").arg(secDiff).arg(secDiff != 1 ? "sekunder" : "sekund");
        else if (secDiff < HOUR)
            return QString::fromUtf8("idag för %0 %1 sedan").arg((int)(secDiff/MINUTE)).arg(secDiff != MINUTE ? "minuter" : "minut");
        else
            return QString::fromUtf8("idag för %0 %1 sedan").arg((int)(secDiff/HOUR)).arg(secDiff != HOUR ? "timmar" : "timme");
    }
    else if (dayDiff > -7)
        return "i " + term2.toString("dddd") + "s";
    else
        return term2.toString("d MMM yyyy");
}

QHash<QString,QString> TwitterFeedModel::createTweet(const QString &text, const QString &link, const QString &pubdate) {
    QHash<QString,QString> s = QHash<QString,QString>();

    QLocale eng(QLocale::English);
    QDateTime tweetDT = eng.toDateTime(pubdate.left(pubdate.length()-6), "ddd, dd MMM yyyy hh:mm:ss");
    tweetDT.setTimeSpec(Qt::UTC);

    s.insert("text", text);
    //s.insert("pubdate", pubdate);
    s.insert("pubdate", "Publicerat " + relativeTime(QDateTime::currentDateTime(), tweetDT) + " via <a href=\"http://twitter.com/pirateplay_se\">Twitter</a>");
    s.insert("link", link);

    return s;
}

void TwitterFeedModel::feedDownloaded() {
    int offset = screenName.length() + 2;
    QDomDocument doc;
    doc.setContent(feed);
    QDomNodeList tweetList = doc.elementsByTagName("item");

    if (tweetList.count() > 0) {
        beginInsertRows(QModelIndex(), 0, tweetList.count()-1);
        for(int i = 0; i<tweetList.count(); i++) {
            tweets.append(createTweet(tweetList.item(i).namedItem("description").toElement().text().remove(0, offset).replace(QRegExp("(https?://\\S+)"), "<a href=\"\\1\">\\1</a>"),
                                      tweetList.item(i).namedItem("link").toElement().text(),
                                      tweetList.item(i).namedItem("pubDate").toElement().text()));
        }
        endInsertRows();
        emit dataChanged(createIndex(0, 0), createIndex(tweetList.count(), 2));
    } else {
        beginInsertRows(QModelIndex(), 0, 0);
        tweets.append(createTweet("Inga nyheter funna", "", ""));
        endInsertRows();
        emit dataChanged(createIndex(0, 0), createIndex(1, 2));
    }

    feed->deleteLater();
}
