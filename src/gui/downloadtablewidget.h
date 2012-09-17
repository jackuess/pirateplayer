#ifndef DOWNLOADTABLEWIDGET_H
#define DOWNLOADTABLEWIDGET_H

#include "../models/downloadlistmodel.h"
#include "downloaddelegate.h"

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTableView>

class DownloadTableWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DownloadTableWidget(QWidget *parent = 0);

    void setModel(DownloadListModel *m);
    
signals:
    
public slots:

private:
    QTableView *table;
    QPushButton *abortButton;
    QPushButton *removeButton;
    QPushButton *removeFromDiskButton;
    QPushButton *openButton;
    DownloadListModel *model;
    QItemSelectionModel *selectionModel;

    int currentRow();
    QModelIndex currentStatusIndex();
    bool currentIsDownloading();
    void enableButtons(bool enabled);

private slots:
    void dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight);
    void selectionChanged( const QItemSelection &selected, const QItemSelection &deselected );

    void openCurrentFile();
    void abortCurrent();
    void removeCurrent();
    void removeCurrentFromDisk();
};

#endif // DOWNLOADTABLEWIDGET_H
