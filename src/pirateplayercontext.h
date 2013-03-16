    #ifndef PIRATEPLAYERCONTEXT_H
#define PIRATEPLAYERCONTEXT_H

#include <QObject>
#include <QSettings>
#include <QDateTime>

#include "network/piratenetworkaccessmanager.h"
#include "mediaplayerinterface.h"

class PirateplayerContext : public QObject
{
    Q_OBJECT
public:
    explicit PirateplayerContext(QObject *parent = 0);

    QNetworkAccessManager* getNetworkAccessManager();

    Q_INVOKABLE void quit();
    Q_INVOKABLE void play(const QString &url, const QString &subsUrl, const QString &playerCmd);
    Q_INVOKABLE QVariant getSetting(const QString key, const QVariant defaultValue = QVariant()) const;
    Q_INVOKABLE void setSetting(const QString key, const QVariant value = QVariant());
    Q_INVOKABLE QString applicationVersion() const;
    Q_INVOKABLE QString formatTweetTime(const QString &timeString) const;
    Q_INVOKABLE QString getHomeDir() const;
    Q_INVOKABLE bool mobile() const;
    Q_INVOKABLE bool mac() const;
    Q_INVOKABLE static bool compatibleVersion(const QString &version);
    Q_INVOKABLE QString applicationDirPath() const;
    Q_INVOKABLE static QString ppBase();
    Q_INVOKABLE QString getClipboardText() const;

    Q_INVOKABLE bool pathIsDir(const QString &path) const;
    Q_INVOKABLE bool pathExists(const QString &path) const;
    Q_INVOKABLE QString pathToNativeSeparators(const QString &path) const;
    Q_INVOKABLE QString pathFromNativeSeparators(const QString &path) const;
    Q_INVOKABLE QString baseName(const QString &path) const;
    Q_INVOKABLE QString suffix(const QString &path) const;
    Q_INVOKABLE QString absDir(const QString &path) const;
    Q_INVOKABLE QString nativeSeparator() const;
    Q_INVOKABLE bool removePath(const QString &path) const;
    Q_INVOKABLE bool pathIsWritable(const QString &path) const;
    Q_INVOKABLE void writeToDisk(const QString &path, const QString &data) const;

    Q_INVOKABLE QString getSaveFileName(const QString &startDir, const QString &filter) const;
    Q_INVOKABLE QString getExistingDirectory(const QString &startDir) const;

    Q_INVOKABLE bool confirmOverwrite(const QString &fileName) const;

    static void openUrl(const QString &url);
    
signals:

private:
    QSettings settings;
    QString relativeTime(const QDateTime &term1, const QDateTime &term2) const;
    PirateNetworkAccessManager nam;
    MediaPlayerInterface mediaPlayer;
};

#endif // PIRATEPLAYERCONTEXT_H
