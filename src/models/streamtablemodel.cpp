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
            QString version = attributes.namedItem("required-player-version").toAttr().value();

            row << (quality == "" ? QString::fromUtf8("Okänd") : quality);
            row << streams.item(i).toElement().text();
            row << attributes.namedItem("subtitles").toAttr().value();
            row << attributes.namedItem("suffix-hint").toAttr().value();
            row << (version == "" ? "1" : version);

            streamList.append(row);
        }

        endInsertRows();
        emit dataChanged(createIndex(0, 0), createIndex(lastRow, COLUMN_COUNT-1));
    }
}

QVariant StreamTableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case QualityColumn:
        case UrlColumn:
        case SubtitlesColumn:
        case VersionColumn:
            return QVariant(streamList[index.row()][index.column()]);
        case SuffixHintColumn:
            QString suffixHint = streamList[index.row()][index.column()];
            if (suffixHint != "")
                return QVariant(QString::fromUtf8("Denna video bör ges filändelsen \".") + suffixHint + "\".");
        }
    }

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
