#include "pirateplayerdesktopviewer.h"
#include "../network/downloadlistmodel.h"

#include <QDeclarativeItem>
#include <QDeclarativeContext>
#include <QFileInfo>
#include <QApplication>

PirateplayerDesktopViewer::PirateplayerDesktopViewer(QObject *parent) :
    QObject(parent), _engine(new QDeclarativeEngine(this)), _rootObject(new QDeclarativeItem)
{
    _engine->setParent(this);

    connect(_engine, SIGNAL(quit()), this, SLOT(quit()));
}

void PirateplayerDesktopViewer::addLibraryPath(const QString& lib)
{
    engine()->addImportPath(lib);
}

void PirateplayerDesktopViewer::addPluginPath(const QString& plugin)
{
    engine()->addPluginPath(plugin);
}

bool PirateplayerDesktopViewer::open(const QString& file_or_url)
{
    currentFileOrUrl = file_or_url;

    QUrl url;
    QFileInfo fi(file_or_url);
    if (fi.exists())
        url = QUrl::fromLocalFile(fi.absoluteFilePath());
    else
        url = QUrl(file_or_url);

    delete rootObject();
    engine()->clearComponentCache();
    engine()->setNetworkAccessManagerFactory(&namFactory);
    qmlRegisterType<DownloadListModel>("Pirateplayer", 1, 0, "DownloadList");
    rootContext()->setContextObject(&ppContext);

    QString fileName = url.toLocalFile();
    if (!fileName.isEmpty()) {
        fi.setFile(fileName);
        if (fi.exists()) {
            if (fi.suffix().toLower() != QLatin1String("qml")) {
                qWarning() << "qml cannot open non-QML file" << fileName;
                return false;
            }
        } else {
            qWarning() << "qml cannot find file:" << fileName;
            return false;
        }
    }

    execute(url);

    return true;
}

void PirateplayerDesktopViewer::execute(QUrl url)
{
    _component = new QDeclarativeComponent(_engine, url, this);
    if (!_component->isLoading()) {
        execute2();
    } else {
        QObject::connect(_component, SIGNAL(statusChanged(QDeclarativeComponent::Status)), this, SLOT(execute2()));
    }
}

void PirateplayerDesktopViewer::execute2()
{
    disconnect(_component, SIGNAL(statusChanged(QDeclarativeComponent::Status)), this, SLOT(execute2()));

    if (_component->isError()) {
        onComponentError();
        return;
    }

    QObject *obj = _component->create();
    obj->setParent(_engine);

    if(_component->isError()) {
        onComponentError();
        return;
    }

    _rootObject = qobject_cast<QGraphicsObject*>(obj);
    Q_ASSERT(_rootObject);
    emit statusChanged(_component->status());
}

void PirateplayerDesktopViewer::onComponentError() {
    QList<QDeclarativeError> errorList = _component->errors();
    foreach (const QDeclarativeError &error, errorList) {
        qWarning() << error;
    }
    emit statusChanged(_component->status());
}

void PirateplayerDesktopViewer::quit()
{
    qApp->quit();
}
