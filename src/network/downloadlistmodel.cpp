#include "downloadlistmodel.h"
#include "../network/download.h"
#include "../network/systemdownload.h"
#include "../network/piratenetworkaccessmanager.h"

#include <QStringList>
#include <QDebug>
#include <QtAlgorithms>
#include <QColor>
#include <QDir>

QStringList DownloadListModel::headers = QStringList() << "Status" << "Filnamn" << QString::fromUtf8("Förlopp") << "Nerladdade (MB)";
QStringList DownloadListModel::statusText = QStringList() << "Inte startad" << "Laddar ned" << QString::fromUtf8("Färdig") << "Fel uppstod" << "Avbruten";
int DownloadListModel::statusColor[5] = {Qt::black, Qt::darkBlue, Qt::darkGreen, Qt::red, Qt::darkRed};

DownloadListModel::DownloadListModel(QObject *parent)//, QNetworkAccessManager *n)
    : QAbstractTableModel(parent) {
    //if (n == 0)
        nam = new PirateNetworkAccessManager(this);
    //else
    //    nam = n;

    QHash<int, QByteArray> roles;
    roles[StatusColumn] = "statusCol";
    roles[FileNameColumn] = "fileName";
    roles[ProgressColumn] = "progressCol";
    roles[DownloadedSizeColumn] = "downloadedSize";
    setRoleNames(roles);
    emit countChanged(0);
}

DownloadListModel::~DownloadListModel() {
    qDeleteAll(downloads);
}

int DownloadListModel::rowCount(const QModelIndex &parent) const {
    return downloads.count();
}

int DownloadListModel::columnCount(const QModelIndex &parent) const {
    return COLUMN_COUNT;
}

QVariant DownloadListModel::data(const QModelIndex &index, int role) const {
    AbstractDownload *item = downloads[index.row()];
    int progress;

    if (!index.isValid())
        return QVariant();

//    if (role == Qt::UserRole && index.column() == 0)
//        return QVariant(item->getStatus());
//    if (role == Qt::DisplayRole) {
//        switch (index.column()) {
//        case StatusColumn:
//            return QVariant(DownloadListModel::statusText[item->getStatus()]);
//        case FileNameColumn:
//            return QVariant(QDir::toNativeSeparators(item->getFileName()));
//        case ProgressColumn:
//            progress = item->getProgress();
//            if (progress > -1)
//                return QVariant(progress);
//            else
//                return QVariant(QString::fromUtf8("Okänt"));
//        case DownloadedSizeColumn:
//            return QVariant(item->getBytesRecieved() / 1048576);
//        }
//    }

//    if (role == Qt::ForegroundRole && index.column() == 0)
//        return QColor((Qt::GlobalColor)statusColor[item->getStatus()]);

    if (role == StatusColumn)
        return QVariant(item->getStatus());
    else if (role == FileNameColumn)
        return QVariant(QDir::toNativeSeparators(item->getFileName()));
    else if (role == ProgressColumn) {
        progress = item->getProgress();
        if (progress > -1)
            return QVariant(progress);
        else
            return QVariant(QString::fromUtf8("Okänt"));
    }
    else if (role == DownloadedSizeColumn)
        return QVariant(item->getBytesRecieved() / 1048576);

    return QVariant();
}

QVariant DownloadListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return DownloadListModel::headers[section];
    else
        return QVariant();
}

void DownloadListModel::addDownload(const QUrl &url, const QString &outFileName, const qint64 &delay, const qint64 &duration) {
    AbstractDownload *d = createDownload(url);
    beginInsertRows(QModelIndex(), 0, 0);
    downloads.prepend(d);
    emit countChanged(downloads.count());
    d->downloadToFile(url, outFileName, delay, duration);
    endInsertRows();
    connect(d, SIGNAL(progress()), this, SLOT(onProgress()));
    connect(d, SIGNAL(statusChanged()), this, SLOT(onStatusChanged()));
}

bool DownloadListModel::removeRow(int row) {
    if (row < downloads.count()) {
        if (downloads[row]->getStatus() == Downloading)
            abortDownload(row);
        beginRemoveRows(QModelIndex(), row, row);
        delete downloads.at(row);
        downloads.removeAt(row);
        emit countChanged(downloads.count());
        endRemoveRows();
        return true;
    }
    else
        return false;
}

bool DownloadListModel::abortDownload(int row) {
    if (row < downloads.count()) {
        downloads[row]->abort();
        return true;
    }
    else
        return false;
}

void DownloadListModel::onProgress() {
    int row = downloads.indexOf((Download*)QObject::sender());
    QModelIndex index = createIndex(row, ProgressColumn, 0);
    emit dataChanged(index, index);
    index = createIndex(row, DownloadedSizeColumn, 0);
    emit dataChanged(index, index);
    //int row = downloads.indexOf((Download*)QObject::sender());
    //QModelIndex startIndex = createIndex(row, 0, 0);
    //QModelIndex endIndex = createIndex(row, 3, 0);
}

void DownloadListModel::onStatusChanged() {
    QModelIndex index = createIndex(downloads.indexOf((Download*)QObject::sender()), StatusColumn, 0);
    emit dataChanged(index, index);
}

AbstractDownload* DownloadListModel::createDownload(QUrl url) {
    AbstractDownload *download;
    if((url.toString().indexOf(".m3u8") > -1) ||
            url.scheme() == "mms")
        download = new SystemDownload(this);
    else
        download = new Download(this, nam);
    return download;
}

QVariantMap DownloadListModel::get(int idx) const {
    QVariantMap map;
    foreach(int k, roleNames().keys()) {
        map[roleNames().value(k)] = data(index(idx, 0), k);
    }
    return map;
}
