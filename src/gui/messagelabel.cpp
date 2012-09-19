#include "messagelabel.h"

#include <QTimer>
#include <QDebug>

const QString BASE_STYLE = "border-radius: 5px; border-width: 2px; border-style: solid; padding: 2px; ";
const QString ERROR_STYLE = "background-color: #eaa; border-color: #c99;";

MessageLabel::MessageLabel(QString c, QWidget *parent) :
    QLabel(parent)
{
    context = c;
    setOpenExternalLinks(true);
}

void MessageLabel::setState(int state) {
    i_state = state;
    switch (i_state) {
    case Fetching:
        setText(QString::fromUtf8("Hämtar strömmar för <em><a href=\"%0\">%0</a></em>").arg(context));
        setStyleSheet(BASE_STYLE + "background-color: #eea; border-color: #cc9;");
        break;
    case NoStreamsFound:
        setText(QString::fromUtf8("Inga strömmar funna för <em><a href=\"%0\">%0</a></em>").arg(context));
        setStyleSheet(BASE_STYLE + ERROR_STYLE);
        break;
    case IncompatibleStreams:
        setText(QString::fromUtf8("Pirateplayer kan tyvärr inte ladda ner denna typ av strömmar.<br />Med <a href=\"http://pirateplay.se/app.html#%0\">Appen på Pirateplay.se</a> går den kanske att ladda ner").arg(context));
        setStyleSheet(BASE_STYLE + ERROR_STYLE);
        break;
    }
}

void MessageLabel::deleteLaterThanLater() {
    QTimer::singleShot(3000, this, SLOT(deleteLater()));
}

QString MessageLabel::getContext() {
    return context;
}

void MessageLabel::mousePressEvent(QMouseEvent *ev) {
    if (i_state != Fetching)
        QTimer::singleShot(500, this, SLOT(deleteLater())); // Hack: delay delete 500ms to make sure any clicked links are still activated before we get deleted
}
