//
//  This file is part of QSimpleUpdater
//
//  Copyright (c) 2014 Alex Spataru <alex_spataru@gmail.com>
//
//  Please check the license.txt file for more information.
//

#include "qsimpleupdater.h"

QSimpleUpdater::QSimpleUpdater(QObject *parent) : QObject(parent) 
{
    m_new_version_available = false;
}


QString QSimpleUpdater::changeLog() {
    // Return the contents of the downloaded changelog
    if (!m_changelog.isEmpty()) {
        return m_changelog;
    }

    // If the changelog is empty, we issue a warning message in
    // the console and return an empty string
    else {
        qWarning() << "QSimpleUpdater: change log is empty,"
                   << "did you call setChangelogUrl() and checkForUpdates()?";
        return NULL;
    }
}

void QSimpleUpdater::checkForUpdates() {
    // Only check for updates if we know which file should we download
    if (!m_reference_url.isEmpty()) {

        // Create a new network access manager, which allows us to
        // download our desired file
        QNetworkAccessManager *_manager = new QNetworkAccessManager(this);

        // Compare the downloaded application version with the installed
        // version when the download is finished
        connect(_manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(checkDownloadedVersion(QNetworkReply*)));

        // Ignore any possible SSL errors
        connect(_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                this, SLOT(ignoreSslErrors(QNetworkReply*,QList<QSslError>)));

        // Finally, download the file
        _manager->get (QNetworkRequest (m_reference_url));
    }

    // Issue a warning message in the case that the reference URL is empty...
    else {
        qWarning() << "QSimpleUpdater: Invalid reference URL";
    }
}

QString QSimpleUpdater::latestVersion() {
    // Return the application version referenced by the string
    // that we downloaded
    if (!m_latest_version.isEmpty()) {
        return m_latest_version;
    }

    // Issue a warning message in the case that the downloaded
    // application version string is empty
    else {
        qWarning() << "QSimpleUpdater: latest version is empty,"
                   << "did you call checkForUpdates() and setReferenceUrl()?";
        return NULL;
    }
}

QString QSimpleUpdater::installedVersion() {
    // Return the string issued by the user in the setApplicationVersion() function
    if (!m_installed_version.isEmpty()) {
        return m_installed_version;
    }

    // Issue a warning message in the case that the installed application
    // version is empty
    else {
        qWarning() << "QSimpleUpdater: installed version is empty,"
                   << "did you call setApplicationVersion()?";
        return NULL;
    }
}

void QSimpleUpdater::downloadLatestVersion() {
    // Open the download URL in a web browser
    if (!m_download_url.isEmpty()) {
        QDesktopServices::openUrl(m_download_url);
    }

    // The m_download_url is empty, so we issue another warning message
    else {
        qWarning() << "QSimpleUpdater: cannot download latest version,"
                   << "did you call setDownloadUrl() and checkForUpdates()?";
    }
}

bool QSimpleUpdater::newerVersionAvailable() {
    return m_new_version_available;
}

void QSimpleUpdater::setDownloadUrl(const QString url) {
    // Change the download URL if the issued URL is valid
    if (!url.isEmpty()) {
        m_download_url.setUrl(url);
    }

    // The issued URL is ilegal, so we warn the user
    else {
        qWarning() << "QSimpleUpdater: input URL cannot be empty!";
    }
}

void QSimpleUpdater::setReferenceUrl(const QString url) {
    // Change the reference URL if the issued URL is valid
    if (!url.isEmpty()) {
        m_reference_url.setUrl(url);
    }

    // The issued URL is ilegal, so we warn the user
    else {
        qWarning() << "QSimpleUpdater: input URL cannot be empty!";
    }
}

void QSimpleUpdater::setChangelogUrl(const QString url) {
    // Change the changelog URL if the issued URL is valid
    if (!url.isEmpty()) {
        m_changelog_url.setUrl(url);
    }

    // The issued URL is ilegal, so we warn the user
    else {
        qWarning() << "QSimpleUpdater: input URL cannot be empty!";
    }
}

void QSimpleUpdater::setApplicationVersion(const QString version) {
    // Change the installed application version if the issued string is valid
    if (!version.isEmpty()) {
        m_installed_version = version;
    }

    // The application version cannot be empty, so we warn the user
    else {
        qWarning() << "QSimpleUpdater: input string cannot be empty!";
    }
}


void QSimpleUpdater::checkDownloadedVersion(QNetworkReply *reply) {
    bool _new_update = false;

    // Read the reply from the server and transform it
    // to a QString
    QString _reply = QString::fromUtf8 (reply->readAll());
    _reply.replace(" ", "");
    _reply.replace("\n", "");

    // If the reply from the server is not empty, compare
    // the downloaded version with the installed version
    if (!_reply.isEmpty() && _reply.contains(".")) {

        // Replace the latest version string with the downloaded string
        m_latest_version = _reply;

        // Separate the downloaded and installed version
        // string by their dots.
        //
        // For example, 0.9.1 would become:
        // 1: 0
        // 2: 9
        // 3: 1
        //
        QStringList _download = m_latest_version.split (".");
        QStringList _installed = m_installed_version.split (".");

        // Compare the major, minor, build, etc. numbers
        for (int i = 0; i <= _download.count() - 1; ++i) {

            // Make sure that the number that we are goind to compare
            // exists in both strings, for example, we will not compare
            // 1.2.3 and 1.2.3.1 because we would crash the program
            if (_download.count() - 1 >= i && _installed.count() - 1 >= i) {

                // The downloaded number is greater than the installed number
                // in question. So there's a newer version of the application
                // available.
                if (_download.at (i) > _installed.at (i)) {
                    _new_update = true;
                    break;
                }
            }

            // If the number of dots are different, we can tell if
            // there's a newer version by comparing the count of each
            // version. For example, 1.2.3 is smaller than 1.2.3.1...
            // Also, we will only reach this code when we finish comparing
            // the "3" in both the downloaded and the installed version.
            else {
                if (_installed.count() < _download.count()) {
                    _new_update = true;
                    break;
                }
            }
        }
    }

    // Update the value of the m_new_version_avialable boolean
    m_new_version_available = _new_update;

    // Notify our parent that we have finished downloading and comparing
    // the application version
    emit versionCheckFinished();

    // If the changelog URL is valid, download the change log ONLY if
    // there's a newer version available.
    // Note that the processDownloadedChangeLog() function will
    // notify our parent that we have finished checking for updates.
    if (!m_changelog_url.isEmpty() && newerVersionAvailable()) {
        QNetworkAccessManager *_manager = new QNetworkAccessManager(this);

        connect(_manager, SIGNAL(finished(QNetworkReply*)),
                this, SLOT(processDownloadedChangelog(QNetworkReply*)));

        connect(_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
                this, SLOT(ignoreSslErrors(QNetworkReply*,QList<QSslError>)));

        _manager->get (QNetworkRequest (m_changelog_url));
    }

    // We did not download the changelog, so we notify our parent
    // that we have finished checking for updates
    else {
        emit checkingFinished();
    }
}

void QSimpleUpdater::processDownloadedChangelog(QNetworkReply *reply) {
    // Read the downloaded file and transform it to a QString
    QString _reply = QString::fromUtf8(reply->readAll());

    // Change the changelog string and notify our
    // parent that the changelog was downlaoded
    if (!_reply.isEmpty()) {
        m_changelog = _reply;
        emit changelogDownloadFinished();
    }

    // Issue a warning in the case that the changelog is empty
    else {
        qWarning() << "QSimpleUpdater: downloaded change log is empty!";
    }

    // Tell our parent that we are done checking for updates
    emit checkingFinished();
}

void QSimpleUpdater::ignoreSslErrors (QNetworkReply *reply, QList<QSslError> error) {
    reply->ignoreSslErrors (error);
}
