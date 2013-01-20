#include "tidynetworkreply.h"

#include <QTimer>
#include <QDebug>

TidyNetworkReply::TidyNetworkReply(const QNetworkRequest &req, QObject *parent) :
    QNetworkReply(parent)
{
    output = TidyBuffer();
    errbuf = TidyBuffer();
    tdoc = tidyCreate();

    offset = 0;

    nam = new QNetworkAccessManager();
    reply = nam->get(req);
    connect(reply, SIGNAL(finished()), this, SLOT(tidyUp()));
}

TidyNetworkReply::~TidyNetworkReply() {
    delete nam;
    tidyBufFree( &output );
    tidyBufFree( &errbuf );
    tidyRelease( tdoc );
}

void TidyNetworkReply::tidyUp() {
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

    if ( rc >= 0 )
    {
        if ( rc > 0 )
            ;//printf( "\nDiagnostics:\n\n%s", errbuf.bp );
    }
    else
        ;//printf( "A severe error (%d) occurred.\n", rc );

    open(ReadOnly);
    emit readyRead();
    emit finished();
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

    return number;
}

void TidyNetworkReply::abort() {
    reply->abort();
}

bool TidyNetworkReply::isSequential() const {
    return false;
}
