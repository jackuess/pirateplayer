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

    emit titleSet("Laddar tillägg");

    QNetworkReply *reply = nam->get(QNetworkRequest(QUrl(QString("http://pirateplay.se/static/pirateplayer_addons/%1/package_info").arg(name))));
    connect(reply, SIGNAL(finished()), SLOT(checkForUpdate()));

    setLayout(new QVBoxLayout(this));
}

void Addon::checkForUpdate() {
    QNetworkReply *reply = (QNetworkReply*)QObject::sender();
    if (reply->error() == QNetworkReply::NoError) {
        pendingMd5 = reply->readLine(reply->size()).trimmed();
        QByteArray reqPlayerVer = reply->readLine(reply->size()).trimmed();

        if (MainWindow::versionNumberGreater(reqPlayerVer, QCoreApplication::instance()->applicationVersion())) {
            QLabel *label = new QLabel(QString::fromUtf8("Väntande uppdatering av detta tillägg är inkompatibelt med din version av Pirateplayer. Vänligen <a href=\"http://pirateplay.se/player.html#install\">uppdatera</a>."), this);
            label->setOpenExternalLinks(true);
            layout()->addWidget(label);
        }
        else if (pendingMd5 != settings->value("addon_" + name + "/md5sum", "0").toByteArray()) {
            download();
            return;
        }
    }

    load();
}

void Addon::download() {
    QNetworkReply *reply = nam->get(QNetworkRequest(QUrl(QString("http://pirateplay.se/static/pirateplayer_addons/%1/package.tar.gz").arg(name))));
    connect(reply, SIGNAL(finished()), SLOT(upgrade()));
}

//TODO: Check that this really works
void Addon::upgrade() {
    //QByteArray *data = new QByteArray(((QIODevice*)QObject::sender())->readAll());
    QNetworkReply *reply = (QNetworkReply*)QObject::sender();

    if (reply->error() == QNetworkReply::NoError) {
        QByteArray *data = new QByteArray((reply->readAll()));
        ArchiveExtractor *extractor = new ArchiveExtractor(this);

        if (QCryptographicHash::hash(*data, QCryptographicHash::Md5).toHex() != pendingMd5) {
            load();
            return;
        }
        settings->setValue("addon_" + name + "/md5sum", pendingMd5);

        removeDir(dir);
        extractor->extractToDir(data, dir);
        connect(extractor, SIGNAL(finished()), SLOT(load()));
    } else {
        load();
    }
}

void Addon::loadingError() {
    emit titleSet(QString::fromUtf8("Tilägg utan titel"));
    QLabel *lbl = new QLabel(QString::fromUtf8("<center>Ett fel uppstod tyvärr när tillägget laddades.</center>"), this);
    ((QVBoxLayout*)layout())->addWidget(lbl, 1);
}

void Addon::load() {
    QString mainQml = this->dir + "/main.qml";

    if (QFile::exists(mainQml)) {
        declarativeView = new QDeclarativeView(this);
        QDeclarativeEngine *engine = declarativeView->engine();
        QDeclarativeContext *rootContext = engine->rootContext();

        engine->setNetworkAccessManagerFactory(new NetworkAccessManagerFactory);

        QString cwd = !this->dir.startsWith('/') ? this->dir = "/" + this->dir : this->dir;
        rootContext->setContextProperty("cwd", cwd);
        rootContext->setContextProperty("mainWindow", this->mainWindow);

        connect(declarativeView, SIGNAL(statusChanged(QDeclarativeView::Status)), SLOT(onViewStatusChange(QDeclarativeView::Status)));

        declarativeView->setResizeMode(QDeclarativeView::SizeRootObjectToView);
        declarativeView->setStyleSheet("background-color:transparent;");
        declarativeView->setSource(QUrl::fromLocalFile(mainQml));
    } else { loadingError(); }
}

void Addon::onViewStatusChange(QDeclarativeView::Status status) {
    if (status == QDeclarativeView::Ready) {
        QObject *rootItem = declarativeView->rootObject();
        QVariant title = rootItem->property("title");
        if (title.isValid())
            emit titleSet(title.toString());
        else
            emit titleSet(QString::fromUtf8("Tilägg utan titel"));

        ((QVBoxLayout*)layout())->addWidget(declarativeView, 1);
    } else if (status == QDeclarativeView::Error) {
        loadingError();
    }
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
