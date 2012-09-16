#ifndef PROGRESSWIDGETDELEGATE_H
#define PROGRESSWIDGETDELEGATE_H

#include <QStyledItemDelegate>

class ProgressWidgetDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit ProgressWidgetDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    
signals:
    
public slots:
    
};

#endif // PROGRESSWIDGETDELEGATE_H
