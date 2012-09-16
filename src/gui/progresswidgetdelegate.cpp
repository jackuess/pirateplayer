#include "progresswidgetdelegate.h"

#include <QApplication>

ProgressWidgetDelegate::ProgressWidgetDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

void ProgressWidgetDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const {
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
