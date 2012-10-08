
#include "windows.h"
#include "winbase.h"

#include "addon.h"

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QVBoxLayout>
#include <QDesktopServices>
#include <QCryptographicHash>
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QtDeclarative>
#include <QLabel>
#include <QDir>

#include "../extra/archiveextractor.h"
#include "../network/piratenetworkaccessmanager.h"

Addon::Addon(const QString &name, QNetworkAccessManager *nam, QSettings *settingsObj, MainWindow *mainWindow, QWidget *parent) :
    QWidget(parent)
{
    this->nam = nam;
    this->settings = settingsObj;
    this->name = name;
    this->dir = QDir::fromNativeSeparators(QString("%1/addons/%2").arg(QDesktopServices::storageLocation(QDesktopServices::DataLocation), name));
    qDebug() << "Setting addon directory:" << this->dir;
    this->mainWindow = mainWindow;

    QNetworkReply *reply = nam->get(QNetworkRequest(QUrl(QString("http://pirateplay.se:8080/static/pirateplayer_addons/%1/package_info").arg(name))));
    connect(reply, SIGNAL(finished()), SLOT(checkForUpdate()));

    setLayout(new QVBoxLayout(this));
}

void Addon::checkForUpdate() {
    QNetworkReply *reply = (QNetworkReply*)QObject::sender();
    pendingMd5 = reply->readLine(reply->size()).trimmed();
    QByteArray reqPlayerVer = reply->readLine(reply->size()).trimmed();

    if (MainWindow::versionNumberGreater(reqPlayerVer, QCoreApplication::instance()->applicationVersion())) {
        QLabel *label = new QLabel(QString::fromUtf8("Väntande uppdatering av detta tillägg är inkompatibelt med din version av Pirateplayer. Vänligen <a href=\"http://pirateplay.se/player.html#install\">uppdatera</a>."), this);
        label->setOpenExternalLinks(true);
        layout()->addWidget(label);
        load();
    }
    else if (pendingMd5 != settings->value("addon_" + name + "/md5sum", "0").toByteArray()) {
        download();
    }
    else {
        load();
    }
}

void Addon::download() {
    QNetworkReply *reply = nam->get(QNetworkRequest(QUrl(QString("http://pirateplay.se:8080/static/pirateplayer_addons/%1/package.tar.gz").arg(name))));
    connect(reply, SIGNAL(finished()), SLOT(upgrade()));
}

void Addon::upgrade() {
    QByteArray *data = new QByteArray(((QIODevice*)QObject::sender())->readAll());
    ArchiveExtractor *extractor = new ArchiveExtractor(this);

    if (QCryptographicHash::hash(*data, QCryptographicHash::Md5).toHex() != pendingMd5) {
        load();
        return;
    }
    settings->setValue("addon_" + name + "/md5sum", pendingMd5);

    removeDir(dir);
    extractor->extractToDir(data, dir);
    connect(extractor, SIGNAL(finished()), SLOT(load()));
}

void Addon::load() {
    QDeclarativeView *view = new QDeclarativeView(this);
    QDeclarativeEngine *engine = view->engine();
    QDeclarativeContext *rootContext = engine->rootContext();

    engine->setNetworkAccessManagerFactory(new NetworkAccessManagerFactory);

    QString cwd = !this->dir.startsWith('/') ? this->dir = "/" + this->dir : this->dir;
    rootContext->setContextProperty("cwd", cwd);
    rootContext->setContextProperty("mainWindow", this->mainWindow);

    view->setResizeMode(QDeclarativeView::SizeRootObjectToView);
    view->setStyleSheet("background-color:transparent;");
    view->setSource(QUrl::fromLocalFile(this->dir + "/main.qml"));

    QObject *rootItem = view->rootObject();
    emit titleSet(rootItem->property("title").toString());

    ((QVBoxLayout*)layout())->addWidget(view, 1);
}

bool Addon::removeDir(const QString &dirName)
{
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


QNetworkAccessManager *NetworkAccessManagerFactory::create(QObject *parent) {
    return new PirateNetworkAccessManager(parent);
}
