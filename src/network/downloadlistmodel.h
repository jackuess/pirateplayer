#ifndef DOWNLOADLISTMODEL_H
#define DOWNLOADLISTMODEL_H

#include <QAbstractTableModel>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QTimer>

class AbstractDownload;

class DownloadListModel : public QAbstractTableModel
{
    Q_OBJECT

    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_ENUMS(Status)

public:
    DownloadListModel(QObject *parent = 0);//, QNetworkAccessManager *n = 0);
    ~DownloadListModel();


    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const;
    Q_INVOKABLE bool removeRow(int row);
    Q_INVOKABLE bool abortDownload(int row);
    Q_INVOKABLE void addDownload(const QUrl &url, const QString &outFileName, const qint64 &delay = 0, const qint64 &duration = 0);
    Q_INVOKABLE QVariantMap get(int idx) const;

    enum Column {
        StatusColumn = 0,
        FileNameColumn,
        ProgressColumn,
        DownloadedSizeColumn,
        COLUMN_COUNT = 4
    };

    enum Status {
        NotStarted = 0,
        Downloading,
        Finished,
        Error,
        Aborted
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

signals:
    void countChanged(int);
};

class AbstractDownload : public QObject
{
    Q_OBJECT

public:
    explicit AbstractDownload(QObject *parent = 0):QObject(parent) { downloadProgress = 0; bytesRecieved = 0; status = DownloadListModel::NotStarted; }

    void downloadToFile(const QUrl &url, const QString &fileName, const qint64 &delay = 0, const qint64 &duration = 0) {
        outFileName = fileName;
        _url = url;
        _duration = duration;
        QTimer::singleShot(delay, this, SLOT(startDownload()));
    }

    QString getUrl() { return _url.toString(); }
    QString getFileName() { return outFileName; }
    int getProgress() { return downloadProgress; }
    int getBytesRecieved() { return bytesRecieved; }
    DownloadListModel::Status getStatus() { return status; }

signals:
    void progress();
    void statusChanged();

public slots:
    virtual void abort() {}

protected slots:
    virtual void startDownload() {
        status = DownloadListModel::Downloading;
        emit statusChanged();

        if (_duration > 0)
            QTimer::singleShot(_duration, this, SLOT(abort()));
    }

protected:
    qint64 _duration;
    QString outFileName;
    QUrl _url;
    int downloadProgress;
    int bytesRecieved;
    DownloadListModel::Status status;
};

#endif // DOWNLOADLISTMODEL_H
