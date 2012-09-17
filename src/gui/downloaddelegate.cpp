#include "downloaddelegate.h"
#include "../models/downloadlistmodel.h"

#include <QApplication>

DownloadDelegate::DownloadDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void DownloadDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QStyleOptionProgressBar progressBarOption;
    int progress;

    switch (index.column()) {
    case DownloadListModel::ProgressColumn:
        progress = index.data().toInt();
        progressBarOption.rect = option.rect;
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.progress = progress;
        progressBarOption.text = QString::number(progress) + "%";
        progressBarOption.textVisible = true;

        QApplication::style()->drawControl(QStyle::CE_ProgressBar,
                                           &progressBarOption, painter);
        return;

    default:
        QStyledItemDelegate::paint(painter, option, index);
    }
}
