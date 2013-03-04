#include "pirateplayercontext.h"

#include <QProcess>
#include <QInputDialog>
#include <QLocale>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <QApplication>

#include "network/tempplayer.h"

#ifdef Q_OS_ANDROID
#include "jni.h"
static JavaVM* s_javaVM = 0;
static jclass s_javaInterfaceClassID = 0;
static jmethodID javaInterfaceOpenURLID = 0;
static QString externalFilesDir;
#endif

PirateplayerContext::PirateplayerContext(QObject *parent) :
    QObject(parent)
{
    ;
}

QNetworkAccessManager* PirateplayerContext::getNetworkAccessManager() {
    return &nam;
}

void PirateplayerContext::quit() {
    QApplication::exit();
}

void PirateplayerContext::play(const QString &url, const QString &subsUrl) {
#if defined( Q_OS_ANDROID ) || defined( EMULATE_ANDROID )
    if (url.startsWith("rtmp")) {
        TempPlayer *tmpPlayer = new TempPlayer(&nam, this);
#ifdef Q_OS_ANDROID
        tmpPlayer->setTempDir(getHomeDir());
#endif
        tmpPlayer->play(url);
    }
    else
        QDesktopServices::openUrl(QUrl(url));
#else
    QProcess *cmd = new QProcess(this);

    bool ok = 0;
    QString cmdTemplate = QInputDialog::getText((QWidget*)this->parent(), tr("Ange uppspelningskommando"),
                                            QString::fromUtf8("Uppspelningskommando:\n    %0 ersätts med videoaddress.\n    %1 ersätts med undertextaddress."), QLineEdit::Normal,
                                            settings.value("Location/player_cmd").toString(), &ok);

    if (ok && !cmdTemplate.isEmpty())
        cmd->start(cmdTemplate.arg(url, subsUrl));
#endif
}

QVariant PirateplayerContext::getSetting(const QString key, const QVariant defaultValue) const {
    return settings.value(key, defaultValue);
}

void PirateplayerContext::setSetting(const QString key, const QVariant value) {
    settings.setValue(key, value);
}

bool PirateplayerContext::compatibleVersion(const QString &version) {
    QStringList leftHandList = version.split('.');
    QStringList rightHandList = QApplication::applicationVersion().split('.');

    for (int i=0; i<leftHandList.count(); i++) {
        if (i == rightHandList.count() ||
            leftHandList.at(i).toInt() > rightHandList.at(i).toInt())
            return false;
        else if (leftHandList.at(i).toInt() < rightHandList.at(i).toInt())
            return true;
    }

    return true;
}

QString PirateplayerContext::applicationVersion() const {
    return QApplication::applicationVersion();
}

QString PirateplayerContext::relativeTime(const QDateTime &term1, const QDateTime &term2) const {
    int dayDiff = term1.daysTo(term2);

    if (dayDiff == 0) {
        int secDiff = -term1.secsTo(term2);
        const int MINUTE = 60;
        const int HOUR = 3600;
        if (secDiff < MINUTE)
            return QString::fromUtf8("idag för %0 %1 sedan").arg(secDiff).arg(secDiff != 1 ? "sekunder" : "sekund");
        else if (secDiff < HOUR)
            return QString::fromUtf8("idag för %0 %1 sedan").arg((int)(secDiff/MINUTE)).arg(secDiff != MINUTE ? "minuter" : "minut");
        else
            return QString::fromUtf8("idag för %0 %1 sedan").arg((int)(secDiff/HOUR)).arg(secDiff != HOUR ? "timmar" : "timme");
    }
    else if (dayDiff > -7)
        return "i " + term2.toString("dddd") + "s";
    else
        return term2.toString("d MMM yyyy");
}

QString PirateplayerContext::formatTweetTime(const QString &timeString) const {
    QLocale eng(QLocale::English);
    QDateTime tweetDT = eng.toDateTime(timeString, "ddd MMM dd hh:mm:ss +0000 yyyy");
    tweetDT.setTimeSpec(Qt::UTC);
    return relativeTime(QDateTime::currentDateTime(), tweetDT);
}

bool PirateplayerContext::pathIsDir(const QString &path) const {
    QFileInfo info(path);
    return info.isDir();
}

bool PirateplayerContext::pathExists(const QString &path) const {
    QFileInfo info(path);
    return info.exists();
}

QString PirateplayerContext::baseName(const QString &path) const {
    QFileInfo info(path);
    return info.baseName();
}

QString PirateplayerContext::absDir(const QString &path) const {
    QFileInfo info(path);
    return info.absolutePath();
}

QString PirateplayerContext::nativeSeparator() const {
    return QDir::separator();
}

QString PirateplayerContext::suffix(const QString &path) const {
    QFileInfo info(path);
    return info.suffix();
}

QString PirateplayerContext::getSaveFileName(const QString &startDir, const QString &filter) const {
    QString selectedFilter;
    QDir dir(startDir);
    QString path = QFileDialog::getSaveFileName(/*(QWidget*)this->parent()*/0, "Spara fil", dir.path(), filter, &selectedFilter, QFileDialog::DontConfirmOverwrite);
    QFileInfo file(path);
    if(file.suffix().isEmpty() && path != "") {
        selectedFilter = selectedFilter.mid(selectedFilter.lastIndexOf('.'));
        selectedFilter = selectedFilter.left(selectedFilter.lastIndexOf(')'));
        path += selectedFilter;
    }
    return path;
}

QString PirateplayerContext::getExistingDirectory(const QString &startDir) const {
    return QFileDialog::getExistingDirectory(QApplication::activeWindow(), QString::fromUtf8("Välj katalog"), startDir);
}

bool PirateplayerContext::confirmOverwrite(const QString &fileName) const {
    QMessageBox messageBox;
    messageBox.setInformativeText(QString::fromUtf8("Vill du ändå spara till den och därmed skriva över den?"));
    messageBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    messageBox.setDefaultButton(QMessageBox::Cancel);

    if (pathExists(fileName)) {
        messageBox.setText("Filen " + QDir::toNativeSeparators(fileName) + " finns sedan tidigare.");
        int ret = messageBox.exec();
        if (ret == QMessageBox::Cancel)
            return false;
    }

    return true;
}

bool PirateplayerContext::removePath(const QString &path) const {
    return QFile::remove(path);
}

bool PirateplayerContext::pathIsWritable(const QString &path) const {
    return QFileInfo(path).isWritable();
}

void PirateplayerContext::writeToDisk(const QString &path, const QString &data) const {
    QFile file(path);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    file.write(data.toLocal8Bit());
    file.close();
}

QString PirateplayerContext::getHomeDir() const {
#ifdef Q_OS_ANDROID
    //return getenv("EXTERNAL_STORAGE");
    return externalFilesDir;
#else
    return QDesktopServices::storageLocation(QDesktopServices::HomeLocation);
#endif
}

bool PirateplayerContext::mobile() const {
#if defined( Q_OS_ANDROID ) || defined( EMULATE_ANDROID )
    return true;
#else
    return false;
#endif
}

void PirateplayerContext::openUrl(const QString &url) {
#ifdef Q_OS_ANDROID
        ////am start -a android.intent.action.VIEW -t video/flv -d file://
        JNIEnv* env;
        if (s_javaVM->AttachCurrentThread(&env, NULL)<0)
        {
            qCritical()<<"AttachCurrentThread failed";
            return;
        }

        jstring str = env->NewString(reinterpret_cast<const jchar*>(url.constData()), url.length());
        env->CallStaticVoidMethod(s_javaInterfaceClassID, javaInterfaceOpenURLID, str);

        s_javaVM->DetachCurrentThread();
#else
        QDesktopServices::openUrl(QUrl(url));
#endif
}

#ifdef Q_OS_ANDROID
//JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
Q_DECL_EXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        qCritical()<<"Can't get the enviroument";
        return -1;
    }

    s_javaVM = vm;

    jclass clazz=env->FindClass("org/kde/necessitas/origo/QJavaInterface");
    if (!clazz)
    {
        qCritical()<<"Can't find QJavaInterface class";
        return -1;
    }

    s_javaInterfaceClassID = (jclass)env->NewGlobalRef(clazz);

    jmethodID getExternalFilesDirID = env->GetStaticMethodID(clazz, "getExternalFilesDir", "()Ljava/lang/String;");
    if (!getExternalFilesDirID)
    {
        qCritical()<<"Can't find getExternalFilesDir method";
        return -1;
    }
    jobject strObj = env->CallStaticObjectMethod(clazz, getExternalFilesDirID);
    const char* str = env->GetStringUTFChars((jstring) strObj, NULL);
    externalFilesDir = QString(str);

    javaInterfaceOpenURLID = env->GetStaticMethodID(clazz, "openURL", "(Ljava/lang/String;)V");
    if (!javaInterfaceOpenURLID)
    {
        qCritical()<<"Can't find openURL method";
        return -1;
    }

    return JNI_VERSION_1_6;
}
#endif
