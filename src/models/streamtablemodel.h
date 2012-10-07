#ifndef STREAMTABLEMODEL_H
#define STREAMTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QStringList>
#include <QNetworkReply>

class StreamTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StreamTableModel(QNetworkReply *xmlData, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int rowCount() const;
    int columnCount(const QModelIndex &parent) const;

    QVariantMap metaData();
    void setMetaData(const QVariantMap &newData);

    enum Column {
        QualityColumn = 0,
        UrlColumn,
        SubtitlesColumn,
        SuffixHintColumn,
        VersionColumn,
        COLUMN_COUNT
    };
    
signals:
    void finished();
    void noStreamsFound();

private:
    QList<QStringList> streamList;
    QNetworkReply *dataDevice;
    QVariantMap private_metaData;

private slots:
    void onData();
    
};

#endif // STREAMTABLEMODEL_H
