#ifndef Q_SIMPLE_UPDATER_H
#define Q_SIMPLE_UPDATER_H

#include <QSsl>
#include <QUrl>
#include <QString>
#include <QSslError>
#include <QByteArray>
#include <QStringList>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDesktopServices>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

class QSimpleUpdater : public QObject {
    Q_OBJECT

public:
    QSimpleUpdater(QObject *parent = 0);

    QString changeLog();
    void checkForUpdates();
    QString latestVersion();
    QString installedVersion();
    void downloadLatestVersion();
    bool newerVersionAvailable();

public slots:
    void setDownloadUrl(const QString url);
    void setReferenceUrl(const QString url);
    void setChangelogUrl(const QString url);
    void setApplicationVersion(const QString version);

private slots:
    void checkDownloadedVersion(QNetworkReply *reply);
    void processDownloadedChangelog(QNetworkReply *reply);
    void ignoreSslErrors(QNetworkReply *reply, QList<QSslError> error);

signals:
    void checkingFinished();
    void versionCheckFinished();
    void changelogDownloadFinished();

private:    
    QString m_changelog;
    QString m_latest_version;
    QString m_installed_version;

    QUrl m_download_url;
    QUrl m_reference_url;
    QUrl m_changelog_url;

    bool m_changelog_downloaded;
    bool m_version_check_finished;

    bool m_new_version_available;
};

#endif
