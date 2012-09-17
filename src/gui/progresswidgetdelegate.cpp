#include "progresswidgetdelegate.h"
#include "../models/downloadlistmodel.h"

#include <QApplication>

ProgressWidgetDelegate::ProgressWidgetDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void ProgressWidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    if (index.column() == DownloadListModel::ProgressColumn) {
        int progress = index.data().toInt();

        QStyleOptionProgressBar progressBarOption;
        progressBarOption.rect = option.rect;
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.progress = progress;
        progressBarOption.text = QString::number(progress) + "%";
        progressBarOption.textVisible = true;

        QApplication::style()->drawControl(QStyle::CE_ProgressBar,
                                           &progressBarOption, painter);
    }
    else
        QStyledItemDelegate::paint(painter, option, index);
}
