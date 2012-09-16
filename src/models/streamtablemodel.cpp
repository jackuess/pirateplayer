#include "streamtablemodel.h"

#include <QDebug>
#include <QDomDocument>

StreamTableModel::StreamTableModel(QIODevice *xmlData, QObject *parent) :
    QAbstractTableModel(parent)
{
    QDomDocument doc;
    doc.setContent(xmlData);

    QDomNodeList streams = doc.elementsByTagName("stream");
    int lastRow = streams.count()-1;

    if (lastRow > -1) {
        beginInsertRows(QModelIndex(), 0, lastRow);

        for(int i = 0; i<=lastRow; i++) {
            QStringList row;
            QDomNamedNodeMap attributes = streams.item(i).attributes();
            QString quality = attributes.namedItem("quality").toAttr().value();

            row << (quality == "" ? QString::fromUtf8("Okänd") : quality);
            row << streams.item(i).toElement().text();
            row << attributes.namedItem("subtitles").toAttr().value();

            streamList.append(row);
        }

        endInsertRows();
        emit dataChanged(createIndex(0, 0), createIndex(lastRow, COLUMN_COUNT-1));
    }
}

QVariant StreamTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole)
        return QVariant(streamList[index.row()][index.column()]);

    return QVariant();
}

int StreamTableModel::rowCount() const {
    return streamList.count();
}

int StreamTableModel::rowCount(const QModelIndex &parent) const {
    return rowCount();
}

int StreamTableModel::columnCount(const QModelIndex &parent) const {
    return COLUMN_COUNT;
}
