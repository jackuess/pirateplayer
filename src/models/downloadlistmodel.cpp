#include "downloadlistmodel.h"
#include "../network/download.h"
#include "../network/systemdownload.h"

#include <QStringList>
#include <QDebug>
#include <QtAlgorithms>
#include <QColor>
#include <QDir>

QStringList DownloadListModel::headers = QStringList() << "Status" << "Filnamn" << QString::fromUtf8("Förlopp");
QStringList DownloadListModel::statusText = QStringList() << "Inte startad" << "Laddar ned" << QString::fromUtf8("Färdig") << "Fel uppstod" << "Avbruten";
int DownloadListModel::statusColor[5] = {Qt::black, Qt::darkBlue, Qt::darkGreen, Qt::red, Qt::darkRed};

DownloadListModel::DownloadListModel(QObject *parent, QNetworkAccessManager *n)
    : QAbstractTableModel(parent) {
    nam = n;
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

    if (!index.isValid())
        return QVariant();

    if (role == Qt::UserRole && index.column() == 0)
        return QVariant(item->getStatus());

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case StatusColumn:
            return QVariant(DownloadListModel::statusText[item->getStatus()]);
        case FileNameColumn:
            return QVariant(QDir::toNativeSeparators(item->getFileName()));
        case ProgressColumn:
            return QVariant(item->getProgress());
        }
    }

    if (role == Qt::ForegroundRole && index.column() == 0)
        return QColor((Qt::GlobalColor)statusColor[item->getStatus()]);

    return QVariant();
}

QVariant DownloadListModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role == Qt::DisplayRole && orientation == Qt::Horizontal)
        return DownloadListModel::headers[section];
    else
        return QVariant();
}

void DownloadListModel::addDownload(QUrl url, QString outFileName) {
    AbstractDownload *d = createDownload(url);
    beginInsertRows(QModelIndex(), 0, 0);
    downloads.prepend(d);
    d->downloadToFile(outFileName);
    endInsertRows();
    connect(d, SIGNAL(progress()), this, SLOT(onProgress()));
    connect(d, SIGNAL(statusChanged()), this, SLOT(onStatusChanged()));
}

bool DownloadListModel::removeRow(int row) {
    if (row < downloads.count()) {
        beginRemoveRows(QModelIndex(), row, row);
        delete downloads.at(row);
        downloads.removeAt(row);
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
    QModelIndex index = createIndex(downloads.indexOf((Download*)QObject::sender()), ProgressColumn, 0);
    emit dataChanged(index, index);
}

void DownloadListModel::onStatusChanged() {
    QModelIndex index = createIndex(downloads.indexOf((Download*)QObject::sender()), StatusColumn, 0);
    emit dataChanged(index, index);
}

AbstractDownload* DownloadListModel::createDownload(QUrl url) {
    AbstractDownload *download;
    if((url.toString().indexOf(".m3u8") > -1) ||
            url.scheme() == "mms")
        download = new SystemDownload(this, url);
    else
        download = new Download(this, nam, url);
    return download;
}
