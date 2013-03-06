#include "maingui.h"
#include "myqmlapplicationviewer.h"
#include "pirateplayerdesktopviewer.h"
#include "../network/downloadlistmodel.h"
#include "../pirateplayercontext.h"

#include <QDir>
#include <QDesktopServices>
#include <QDebug>
#include <QCryptographicHash>
#include <QApplication>

#ifdef Q_OS_ANDROID
#include <QAndroidStyle>
#endif

MainGui::MainGui(QObject *parent) :
    QObject(parent)
{
    extractor = new ArchiveExtractor(this);

    pkgDir = QDir::fromNativeSeparators(QString("%1/gui").arg(QDesktopServices::storageLocation(QDesktopServices::DataLocation)));

#if defined( Q_OS_ANDROID ) || defined( EMULATE_ANDROID )
    qmlViewer = new MyQmlApplicationViewer();
    qmlViewer->addLibraryPath(pkgDir + "/imports/android");
#else
    qmlViewer = new PirateplayerDesktopViewer(this);
    qmlViewer->addLibraryPath(pkgDir + "/imports/desktop");
#endif

    nam = qmlViewer->getPpContext()->getNetworkAccessManager();

    qmlRegisterType<DownloadListModel>("Pirateplayer", 1, 0, "DownloadList");

    pirateplaySeBase = PirateplayerContext::ppBase();

    setFont();
}

MainGui::~MainGui() {
    delete qmlViewer;
}

void MainGui::setFont() {
#ifdef Q_OS_ANDROID
    QFont globalFont = QFont("Roboto");
    globalFont.setPointSize(settings.value("MainWindow/font_point_size", QVariant(6)).toInt());
#else
    QFont globalFont = qApp->font();
    globalFont.setPointSize(settings.value("MainWindow/font_point_size", QVariant(globalFont.pointSize())).toInt());
#endif

    settings.setValue("MainWindow/font_point_size", QVariant(globalFont.pointSize()));
    qApp->setFont(globalFont);
}

void MainGui::show() {
    QNetworkReply *reply = nam->get(QNetworkRequest(QUrl(QString(pirateplaySeBase + "/static/pirateplayer_gui/pkglist"))));
    connect(reply, SIGNAL(finished()), SLOT(findPkg()));
}

void MainGui::findPkg() {
    QNetworkReply *reply = (QNetworkReply*)QObject::sender();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray line = reply->readLine();
        while (line != "") {
            QList<QByteArray> pkgParams = line.split(' ');
            QByteArray ppVersion = pkgParams[0];
            QByteArray md5Sum = pkgParams[1];
            QByteArray fileName = pkgParams[2].trimmed();

            if (PirateplayerContext::compatibleVersion(ppVersion) && md5Sum == settings.value("MainWindow/gui_md5sum", "").toString() && QDir(pkgDir).exists())
                break;
            else if (PirateplayerContext::compatibleVersion(ppVersion)) {
                pendingMd5Sum = md5Sum;
                QNetworkReply *pkgReply = nam->get(QNetworkRequest(QUrl(QString(pirateplaySeBase + "/static/pirateplayer_gui/" + fileName))));
                connect(pkgReply, SIGNAL(finished()), SLOT(extractPkg()));
                reply->deleteLater();
                return;
            }
            line = reply->readLine();
        }
    }

    load();
    reply->deleteLater();
}

void MainGui::extractPkg() {
    QNetworkReply *reply = (QNetworkReply*)QObject::sender();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray *data = new QByteArray((reply->readAll()));

        if (QCryptographicHash::hash(*data, QCryptographicHash::Md5).toHex() != pendingMd5Sum) {
            reply->deleteLater();
            load();
            return;
        }
        settings.setValue("MainWindow/gui_md5sum", pendingMd5Sum);

        removeDir(pkgDir);
        extractor->extractToDir(data, pkgDir);
        connect(extractor, SIGNAL(finished()), SLOT(load()));
    } else {
        load();
    }

    reply->deleteLater();
}

void MainGui::load() {
    QString mainQml = pkgDir + "/main.qml";

    if (QFile::exists(mainQml)) {
        qmlViewer->open(mainQml);
    } else { qDebug() << "main.qml ej funnen."; qApp->quit(); }
}

bool MainGui::removeDir(const QString &dirName) {
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }

    return result;
}
