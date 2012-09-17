#ifndef DOWNLOADLISTMODEL_H
#define DOWNLOADLISTMODEL_H

#include "../network/abstractdownload.h"

#include <QAbstractTableModel>
#include <QNetworkAccessManager>

class DownloadListModel : public QAbstractTableModel
{
    Q_OBJECT

    public:
        DownloadListModel(QObject *parent, QNetworkAccessManager *n);
        ~DownloadListModel();


        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        QVariant data(const QModelIndex &index, int role) const;
        QVariant headerData(int section, Qt::Orientation orientation,
                         int role = Qt::DisplayRole) const;
        bool removeRow(int row);
        bool abortDownload(int row);
        void addDownload(QUrl url, QString outFileName);

        enum Column {
            StatusColumn = 0,
            FileNameColumn,
            ProgressColumn,
            COLUMN_COUNT = 3
        };

 private:
    AbstractDownload* createDownload(QUrl url);

    QList<AbstractDownload*> downloads;
    QNetworkAccessManager *nam;

    static QStringList headers;
    static QStringList statusText;
    static int statusColor[5];

private slots:
    void onProgress();
    void onStatusChanged();
};

#endif // DOWNLOADLISTMODEL_H
