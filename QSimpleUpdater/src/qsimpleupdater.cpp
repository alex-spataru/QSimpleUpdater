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
 */

#include "qsimpleupdater.h"

QSimpleUpdater::QSimpleUpdater (QObject *parent)
    : QObject (parent)
    , m_changelog_downloaded (false)
    , m_version_check_finished (false)
    , m_new_version_available (false) {
    m_downloadDialog = new DownloadDialog();
}

QString QSimpleUpdater::changeLog() const {    
    return m_changelog;
}

void QSimpleUpdater::checkForUpdates(void) {
    if (!m_reference_url.isEmpty()) {
        QNetworkAccessManager *_manager = new QNetworkAccessManager (this);

        connect (_manager, SIGNAL (finished (QNetworkReply *)), this,
                 SLOT (checkDownloadedVersion (QNetworkReply *)));

        connect (_manager, SIGNAL (sslErrors (QNetworkReply *, QList<QSslError>)),
                 this, SLOT (ignoreSslErrors (QNetworkReply *, QList<QSslError>)));

        _manager->get (QNetworkRequest (m_reference_url));
    }

    else
        qDebug() << "QSimpleUpdater: Invalid reference URL";
}

void QSimpleUpdater::openDownloadLink(void) {
    if (!m_download_url.isEmpty())
        QDesktopServices::openUrl (m_download_url);
}

QString QSimpleUpdater::latestVersion() const {
    return m_latest_version;
}

QString QSimpleUpdater::installedVersion() const {
    return m_installed_version;
}

void QSimpleUpdater::downloadLatestVersion(void) {
    if (!m_download_url.isEmpty())
        m_downloadDialog->beginDownload (m_download_url);
}

bool QSimpleUpdater::newerVersionAvailable() const {
    return m_new_version_available;
}

void QSimpleUpdater::setDownloadUrl (const QString& url) {
    Q_ASSERT (!url.isEmpty());
    m_download_url.setUrl (url);
}

void QSimpleUpdater::setReferenceUrl (const QString& url) {
    Q_ASSERT (!url.isEmpty());
    m_reference_url.setUrl (url);
}

void QSimpleUpdater::setChangelogUrl (const QString& url) {
    Q_ASSERT (!url.isEmpty());
    m_changelog_url.setUrl (url);
}

void QSimpleUpdater::setApplicationVersion (const QString& version) {
    Q_ASSERT (!version.isEmpty());
    m_installed_version = version;
}

void QSimpleUpdater::checkDownloadedVersion (QNetworkReply *reply) {
    bool _new_update = false;

    QString _reply = QString::fromUtf8 (reply->readAll());
    _reply.replace (" ", "");
    _reply.replace ("\n", "");

    if (!_reply.isEmpty() && _reply.contains (".")) {
        m_latest_version = _reply;

        QStringList _download = m_latest_version.split (".");
        QStringList _installed = m_installed_version.split (".");

        for (int i = 0; i <= _download.count() - 1; ++i) {
            if (_download.count() - 1 >= i && _installed.count() - 1 >= i) {
                if (_download.at (i) > _installed.at (i)) {
                    _new_update = true;
                    break;
                }
            }

            else {
                if (_installed.count() < _download.count()) {
                    if (_installed.at (i - 1) == _download.at (i - 1))
                        break;

                    else {
                        _new_update = true;
                        break;
                    }
                }
            }
        }
    }

    m_new_version_available = _new_update;
    emit versionCheckFinished();

    if (!m_changelog_url.isEmpty() && newerVersionAvailable()) {
        QNetworkAccessManager *_manager = new QNetworkAccessManager (this);

        connect (_manager, SIGNAL (finished (QNetworkReply *)), this,
                 SLOT (processDownloadedChangelog (QNetworkReply *)));

        connect (_manager, SIGNAL (sslErrors (QNetworkReply *, QList<QSslError>)),
                 this, SLOT (ignoreSslErrors (QNetworkReply *, QList<QSslError>)));

        _manager->get (QNetworkRequest (m_changelog_url));
    }

    else
        emit checkingFinished();
}

void QSimpleUpdater::processDownloadedChangelog (QNetworkReply *reply) {
    QString _reply = QString::fromUtf8 (reply->readAll());

    if (!_reply.isEmpty()) {
        m_changelog = _reply;
        emit changelogDownloadFinished();
    }

    emit checkingFinished();
}

void QSimpleUpdater::ignoreSslErrors (QNetworkReply *reply,
                                      const QList<QSslError>& error) {
#if SUPPORTS_SSL
    reply->ignoreSslErrors (error);
#else
    Q_UNUSED (reply);
    Q_UNUSED (error);
#endif
}
