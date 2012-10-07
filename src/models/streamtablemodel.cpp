#include "streamtablemodel.h"

#include <QDebug>
#include <QDomDocument>

StreamTableModel::StreamTableModel(QNetworkReply *xmlData, QObject *parent) :
    QAbstractTableModel(parent)
{
    this->dataDevice = xmlData;
    connect(this->dataDevice, SIGNAL(finished()), SLOT(onData()));
}

void StreamTableModel::onData() {
    QDomDocument doc;
    doc.setContent(this->dataDevice);

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
        emit finished();
    }
    else {
        emit noStreamsFound();
    }

    this->dataDevice->deleteLater();
}

QVariantMap StreamTableModel::metaData() {
    return this->private_metaData;
}

void StreamTableModel::setMetaData(const QVariantMap &newData) {
    this->private_metaData = newData;
    QVariantMap tmpMap;
    QVariantMap::const_iterator i;
    for (i = this->private_metaData.constBegin(); i != this->private_metaData.constEnd(); ++i) {
        tmpMap[i.key()] =  i.value().toString().replace('/', '-')
                                                .replace(':', '-')
                                                .replace('?', "");
    }
    this->private_metaData.unite(tmpMap);
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
    else if (role == Qt::UserRole && index.column() == SuffixHintColumn)
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
