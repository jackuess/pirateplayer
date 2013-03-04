#include "abstractqmlviewer.h"
#include "qmlapplicationviewer.h"

#include <QSettings>

#ifndef MYQMLAPPLICATIONVIEWER_H
#define MYQMLAPPLICATIONVIEWER_H

class MyQmlApplicationViewer : public QmlApplicationViewer, public AbstractQmlViewer
{
    Q_OBJECT
public:
    explicit MyQmlApplicationViewer(QWidget *parent = 0);
    void showExpanded();

    bool open(const QString &url);
    void addLibraryPath(const QString& lib);
    
signals:
    
public slots:

protected slots:
    void closeEvent(QCloseEvent *);
    void showOnLoaded();

private:
    void installFfmpeg();
};

#endif // MYQMLAPPLICATIONVIEWER_H
