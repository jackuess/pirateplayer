#ifndef MESSAGELABEL_H
#define MESSAGELABEL_H

#include <QWidget>
#include <QLabel>

class MessageLabel : public QLabel
{
    Q_OBJECT
public:
    explicit MessageLabel(QString c, QWidget *parent = 0);
    void setState(int state);
    void deleteLaterThanLater();
    QString getContext();

    enum State {
        Fetching = 0,
        NoStreamsFound,
        IncompatibleStreams
    };
    
private:
    QString context;
    int i_state;

protected:
    void mousePressEvent(QMouseEvent *ev);
};

#endif // MESSAGELABEL_H
