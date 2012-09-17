#ifndef DOWNLOADDELEGATE_H
#define DOWNLOADDELEGATE_H

#include <QStyledItemDelegate>

class DownloadDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit DownloadDelegate(QObject *parent = 0);

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    
signals:
    
public slots:
    
};

#endif // DOWNLOADDELEGATE_H
