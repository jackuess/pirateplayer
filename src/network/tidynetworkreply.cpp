#include "tidynetworkreply.h"

#include <QTimer>
#include <QDebug>

TidyNetworkReply::TidyNetworkReply(const QNetworkRequest &req, QObject *parent) :
    QNetworkReply(parent)
{
    output = {0};
    errbuf = {0};
    tdoc = tidyCreate();

    offset = 0;

    nam = new QNetworkAccessManager();
    reply = nam->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(tidyUp()));
    this->setRequest(req);
}

TidyNetworkReply::~TidyNetworkReply() {
    delete nam;
    if (!this->attribute(QNetworkRequest::RedirectionTargetAttribute).isValid()) {
        tidyBufFree( &output );
        tidyBufFree( &errbuf );
        tidyRelease( tdoc );
    }
}

void TidyNetworkReply::tidyUp() {
    QUrl redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (redirect.isValid()) {
        redirect.setScheme("tidy");
        setAttribute(QNetworkRequest::RedirectionTargetAttribute, QVariant(redirect));

        emit finished();
        reply->deleteLater();
        return;
    }

    int rc = -1;
    Bool ok;

    ok = tidyOptSetBool( tdoc, TidyXmlOut, yes );  // Convert to XHTML
    if (ok)
        ok = tidyOptSetBool(tdoc, TidyQuoteNbsp, no);
    //if (ok)
        //ok = tidyOptSetValue(tdoc, TidyBlockTags, "header,nav,article,time,section,footer");
    if ( ok )
        rc = tidySetErrorBuffer( tdoc, &errbuf );      // Capture diagnostics
    if ( rc >= 0 )
        rc = tidyParseString( tdoc, reply->readAll() );           // Parse the input
    if ( rc >= 0 )
        rc = tidyCleanAndRepair( tdoc );               // Tidy it up!
    if ( rc >= 0 )
        rc = tidyRunDiagnostics( tdoc );               // Kvetch
    if ( rc > 1 )                                    // If error, force output.
        rc = ( tidyOptSetBool(tdoc, TidyForceOutput, yes) ? rc : -1 );
    if ( rc >= 0 )
        rc = tidySaveBuffer( tdoc, &output );          // Pretty Print

    if ( rc >= 0 ) {
        if ( rc > 0 ) {
            ;//printf( "\nDiagnostics:\n\n%s", errbuf.bp );
        }
    } else {
            ;//printf( "A severe error (%d) occurred.\n", rc );
    }

    open(ReadOnly);
    emit readyRead();
    emit finished();

    reply->deleteLater();
    //QTimer::singleShot(0, this, SIGNAL(readyRead()));
    //QTimer::singleShot(0, this, SIGNAL(finished()));
}

qint64 TidyNetworkReply::bytesAvailable() const {
    return reply->bytesAvailable() + QIODevice::bytesAvailable();
    return output.size-offset + QIODevice::bytesAvailable();
}

qint64 TidyNetworkReply::readData(char *data, qint64 maxSize) {
    if (offset >= output.size)
        return -1;

    qint64 number = qMin(maxSize, output.size - offset);
    memcpy(data, output.bp + offset, number);
    offset += number;

//    qDebug() << this->request().url().toString();
//    if (this->request().url().toString().contains("kanal9")) {
//        QFile tmp("/home/chucky/tmp.html");
//        tmp.open(QIODevice::WriteOnly | QIODevice::Append);
//        tmp.write(data);
//        tmp.close();
//    }

    return number;
}

void TidyNetworkReply::abort() {
    reply->abort();
}

bool TidyNetworkReply::isSequential() const {
    return false;
}
