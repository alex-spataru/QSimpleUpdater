#ifndef Q_SIMPLE_UPDATER_H
#define Q_SIMPLE_UPDATER_H

#include <QSsl>
#include <QUrl>
#include <QDebug>
#include <QString>
#include <QSslError>
#include <QByteArray>
#include <QStringList>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDesktopServices>
#include <QSslConfiguration>
#include <QNetworkAccessManager>

#include "dialogs/download_dialog.h"

class QSimpleUpdater : public QObject
{
        Q_OBJECT

    public:
        QSimpleUpdater (QObject *parent = 0);

		/// Returns the downloaded change log
        QString changeLog() const;
		
		/// Returns the downloaded version string
		QString latestVersion() const;
		
		/// Returns the local version, referenced by 
		/// the setApplicationVersion() function
        QString installedVersion() const;
		
		/// Returns \c true if there's a newer version available
	    bool newerVersionAvailable() const;
		
		/// Checks for updates and calls the appropriate
		/// signals when finished
        void checkForUpdates();
		
		/// Opens the download URL in a a web browser.
		/// The URL is referenced by the \c setDownloadUrl() function
        void openDownloadLink();
		
		/// Shows a dialog that downloads the file in the
		/// URL referenced by the \c setDownloadUrl() function
        void downloadLatestVersion();

    public slots:
	
	    /// Changes the URL that we can open in a web browser or
		/// download. Its recommended to use fixed URLs if you 
		/// want to automatically download and install your updates
        void setDownloadUrl (const QString &url);
		
		/// Changes the reference URL, which contains ONLY the latest
		/// version of your application as a plain text file. 
		/// Examples include:
		///     - 1.2.3
		///     - 5.4.0
		///     - 0.1.2
		///     - etc.
        void setReferenceUrl (const QString &url);
		
		/// Changes the change log URL, which contains the change log
		/// of your application. The change log can be any file you
		/// like, however, its recommended to write it in plain text, 
		/// such as TXT, HTML and RTF files.
        void setChangelogUrl (const QString &url);
		
		/// Tells the updater the version of the installed 
		/// copy of your application.
        void setApplicationVersion (const QString &version);

    private slots:
        void checkDownloadedVersion (QNetworkReply *reply);
        void processDownloadedChangelog (QNetworkReply *reply);
        void ignoreSslErrors (QNetworkReply *reply, const QList<QSslError> &error);

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

        DownloadDialog *m_downloadDialog;
};

#endif
