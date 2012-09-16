#ifndef STREAMTABLEMODEL_H
#define STREAMTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QStringList>
#include <QIODevice>

class StreamTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StreamTableModel(QIODevice *xmlData, QObject *parent = 0);

    QVariant data(const QModelIndex &index, int role) const;
    int rowCount(const QModelIndex &parent) const;
    int rowCount() const;
    int columnCount(const QModelIndex &parent) const;

    enum Column {
        QualityColumn = 0,
        UrlColumn,
        SubtitlesColumn,
        COLUMN_COUNT
    };
    
signals:
    
public slots:

private:
    QList<QStringList> streamList;
    
};

#endif // STREAMTABLEMODEL_H
