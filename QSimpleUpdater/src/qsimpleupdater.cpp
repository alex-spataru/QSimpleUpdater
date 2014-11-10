//
//  This file is part of QSimpleUpdater
//
//  Copyright (c) 2014 Alex Spataru <alex_spataru@gmail.com>
//
//  Please check the license.txt file for more information.
//

#include "qsimpleupdater.h"

QSimpleUpdater::QSimpleUpdater (QObject *parent)
    : QObject (parent)
    , m_changelog_downloaded (false)
    , m_version_check_finished (false)
    , m_new_version_available (false)
{
    m_downloadDialog = new DownloadDialog();
}

QString QSimpleUpdater::changeLog() const
{
    if (m_changelog.isEmpty())
    {
        qDebug() << "QSimpleUpdater: change log is empty,"
                 << "did you call setChangelogUrl() and checkForUpdates()?";
    }

    return m_changelog;
}

void QSimpleUpdater::checkForUpdates()
{
    if (!m_reference_url.isEmpty())
    {
        QNetworkAccessManager *_manager = new QNetworkAccessManager (this);

        connect (_manager, SIGNAL (finished (QNetworkReply *)),
                 this, SLOT (checkDownloadedVersion (QNetworkReply *)));

        connect (_manager, SIGNAL (sslErrors (QNetworkReply *, QList<QSslError>)),
                 this, SLOT (ignoreSslErrors (QNetworkReply *, QList<QSslError>)));

        _manager->get (QNetworkRequest (m_reference_url));
    }

    else
        qDebug() << "QSimpleUpdater: Invalid reference URL";
}

void QSimpleUpdater::openDownloadLink()
{
    if (!m_download_url.isEmpty())
        QDesktopServices::openUrl (m_download_url);

    else
    {
        qDebug() << "QSimpleUpdater: cannot download latest version,"
                 << "did you call setDownloadUrl() and checkForUpdates()?";
    }
}

QString QSimpleUpdater::latestVersion() const
{
    if (m_latest_version.isEmpty())
    {
        qDebug() << "QSimpleUpdater: latest version is empty,"
                 << "did you call checkForUpdates() and setReferenceUrl()?";
    }

    return m_latest_version;
}

QString QSimpleUpdater::installedVersion() const
{
    if (m_installed_version.isEmpty())
    {
        qDebug() << "QSimpleUpdater: installed version is empty,"
                 << "did you call setApplicationVersion()?";
    }

    return m_installed_version;
}

void QSimpleUpdater::downloadLatestVersion()
{
    if (!m_download_url.isEmpty())
        m_downloadDialog->beginDownload (m_download_url);

    else
    {
        qDebug() << "QSimpleUpdater: cannot download latest version,"
                 << "did you call setDownloadUrl() and checkForUpdates()?";
    }
}

bool QSimpleUpdater::newerVersionAvailable() const
{
    return m_new_version_available;
}

void QSimpleUpdater::setDownloadUrl (const QString &url)
{
    Q_ASSERT (!url.isEmpty());

    if (!url.isEmpty())
        m_download_url.setUrl (url);

    else
        qDebug() << "QSimpleUpdater: input URL cannot be empty!";
}

void QSimpleUpdater::setReferenceUrl (const QString &url)
{
    Q_ASSERT (!url.isEmpty());

    if (!url.isEmpty())
        m_reference_url.setUrl (url);

    else
        qDebug() << "QSimpleUpdater: input URL cannot be empty!";
}

void QSimpleUpdater::setChangelogUrl (const QString &url)
{
    Q_ASSERT (!url.isEmpty());

    if (!url.isEmpty())
        m_changelog_url.setUrl (url);

    else
        qDebug() << "QSimpleUpdater: input URL cannot be empty!";
}

void QSimpleUpdater::setApplicationVersion (const QString &version)
{
    Q_ASSERT (!version.isEmpty());

    if (!version.isEmpty())
        m_installed_version = version;

    else
        qDebug() << "QSimpleUpdater: input string cannot be empty!";
}

void QSimpleUpdater::checkDownloadedVersion (QNetworkReply *reply)
{
    bool _new_update = false;

    QString _reply = QString::fromUtf8 (reply->readAll());
    _reply.replace (" ", "");
    _reply.replace ("\n", "");

    if (!_reply.isEmpty() && _reply.contains ("."))
    {
        m_latest_version = _reply;
		
        QStringList _download = m_latest_version.split (".");
        QStringList _installed = m_installed_version.split (".");
		
        for (int i = 0; i <= _download.count() - 1; ++i)
        {
            if (_download.count() - 1 >= i && _installed.count() - 1 >= i)
            {
                if (_download.at (i) > _installed.at (i))
                {
                    _new_update = true;
                    break;
                }
            }
			
            else
            {
                if (_installed.count() < _download.count())
                {
                    if (_installed.at (i - 1) == _download.at (i - 1))
                        break;

                    else
                    {
                        _new_update = true;
                        break;
                    }
                }
            }
        }
    }

    m_new_version_available = _new_update;
    emit versionCheckFinished();

    if (!m_changelog_url.isEmpty() && newerVersionAvailable())
    {
        QNetworkAccessManager *_manager = new QNetworkAccessManager (this);

        connect (_manager, SIGNAL (finished (QNetworkReply *)),
                 this, SLOT (processDownloadedChangelog (QNetworkReply *)));

        connect (_manager, SIGNAL (sslErrors (QNetworkReply *, QList<QSslError>)),
                 this, SLOT (ignoreSslErrors (QNetworkReply *, QList<QSslError>)));

        _manager->get (QNetworkRequest (m_changelog_url));
    }

    else
        emit checkingFinished();
}

void QSimpleUpdater::processDownloadedChangelog (QNetworkReply *reply)
{
    QString _reply = QString::fromUtf8 (reply->readAll());

    if (!_reply.isEmpty())
    {
        m_changelog = _reply;
        emit changelogDownloadFinished();
    }

    else
        qDebug() << "QSimpleUpdater: downloaded change log is empty!";
		
    emit checkingFinished();
}

void QSimpleUpdater::ignoreSslErrors (QNetworkReply *reply, const QList<QSslError> &error)
{
    reply->ignoreSslErrors (error);
}