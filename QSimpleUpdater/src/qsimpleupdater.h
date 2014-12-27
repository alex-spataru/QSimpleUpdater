/*
 * (C) Copyright 2014 Alex Spataru
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the GNU Lesser General Public License
 * (LGPL) version 2.1 which accompanies this distribution, and is available at
 * http://www.gnu.org/licenses/lgpl-2.1.html
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 */

#ifndef Q_SIMPLE_UPDATER_H
#define Q_SIMPLE_UPDATER_H

#define SUPPORTS_SSL !defined(Q_OS_IOS)

#include <QUrl>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QDesktopServices>
#include <QNetworkAccessManager>

#if SUPPORTS_SSL
#include <QSsl>
#include <QSslError>
#include <QSslConfiguration>
#endif

#include "dialogs/download_dialog.h"

class QSimpleUpdater : public QObject {
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
    void checkForUpdates(void);

    /// Opens the download URL in a a web browser.
    /// The URL is referenced by the \c setDownloadUrl() function
    void openDownloadLink(void);

    /// Shows a dialog that downloads the file in the
    /// URL referenced by the \c setDownloadUrl() function
    void downloadLatestVersion(void);

public slots:

    /// Changes the URL that we can open in a web browser or
    /// download. Its recommended to use fixed URLs if you
    /// want to automatically download and install your updates
    void setDownloadUrl (const QString& url);

    /// Changes the reference URL, which contains ONLY the latest
    /// version of your application as a plain text file.
    /// Examples include:
    ///     - 1.2.3
    ///     - 5.4.0
    ///     - 0.1.2
    ///     - etc.
    void setReferenceUrl (const QString& url);

    /// Changes the change log URL, which contains the change log
    /// of your application. The change log can be any file you
    /// like, however, its recommended to write it in plain text,
    /// such as TXT, HTML and RTF files.
    void setChangelogUrl (const QString& url);

    /// Tells the updater the version of the installed
    /// copy of your application.
    void setApplicationVersion (const QString& version);

private slots:
    void checkDownloadedVersion (QNetworkReply *reply);
    void processDownloadedChangelog (QNetworkReply *reply);
    void ignoreSslErrors (QNetworkReply *reply, const QList<QSslError>& error);

signals:
    void checkingFinished(void);
    void versionCheckFinished(void);
    void changelogDownloadFinished(void);

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
