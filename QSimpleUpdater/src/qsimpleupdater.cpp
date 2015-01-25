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

/*! \class QSimpleUpdater
 * \brief A simple auto-updater system for Qt
 *
 * QSimpleUpdater is an implementation of an auto-updating system to be used with Qt projects.
 * Aside from notifying you if there's a newer version, it allows you to download the change log in any
 * format (such as HTML or RTF) and download the updates directly from your application using a dialog.
 * QSimpleUpdater is free and open source LGPL software,
 * which means that you can use it for both open source and proprietary applications.
 *
 * \chapter Installation
 *
 * \list
 * \o Copy the QSimpleUpdater folder in your "3rd-party" folder.
 * \o Include the QSimpleUpdater project include (pri) file using the include() function.
 * \o That's all! Check the example project as a reference for your project.
 * \endlist
 *
 * \chapter Running the example project
 *
 * \list
 * \o Navigate to the Example folder and open example.pro with Qt Creator.
 * \o Compile the project and play with it :)
 * \endlist
 *
 * \chapter Warnings
 * Many websites today use the HTTP Secure protocol, which means that you will need SSL
 * in order to communicate with them.
 * If your project needs to access such a webiste (for example GitHub),
 * you will need to carefully read the following information in order to ensure that
 * QSimpleUpdater works with those websites (both in your machine and in the final users' machine).
 *
 * This section is extremely important for any developers wishing to deploy their applications under
 * the Windows platform, because the application will depend on the OpenSSL libaries in order to work.
 *
 * \bold {Linux}
 *
 * Make sure that you have installed the following libraries in your system:
 *
 * \list
 * \o lssl
 * \o lcrypto
 * \endlist
 *
 * \bold {Mac OS X}
 *
 * The libraries required by QSimpleUpdater are the same as Linux, however, these libraries
 * are installed by default in most Mac OS X installations.
 *
 * \bold {Microsoft Windows}
 *
 * QSimpleUpdater makes use of the OpenSSL-Win32 project, make sure that have it installed
 * and that the project knows where to find them (the default location is C:/OpenSSL-Win32).
 * Finally, deploy the following libraries with your compiled project:
 *
 * \list
 * \o libeay32.dll
 * \o ssleay32.dll
 * \endlist
 */

/*! \fn QSimpleUpdater::checkingFinished (void)
 * This signal is triggered when the updater system finishes downloading
 * and proccessing the version data and changelog data.
 */

#include "qsimpleupdater.h"

/*! \internal
 * Initializes and configures the class.
 */

QSimpleUpdater::QSimpleUpdater (QObject *parent)
    : QObject (parent)
    , m_silent (false)
    , m_show_newest_version (true)
    , m_show_update_available (true)
    , m_new_version_available (false)
{

    m_progressDialog = new ProgressDialog();
    m_downloadDialog = new DownloadDialog();

    m_manager = new QNetworkAccessManager (this);
    connect (m_manager, SIGNAL (finished (QNetworkReply *)), this,
             SLOT (checkDownloadedVersion (QNetworkReply *)));
    connect (m_manager, SIGNAL (sslErrors (QNetworkReply *, QList<QSslError>)),
             this, SLOT (ignoreSslErrors (QNetworkReply *, QList<QSslError>)));

    connect (m_progressDialog, SIGNAL (cancelClicked()), this, SLOT (cancel()));
    connect (this, SIGNAL (checkingFinished()), this, SLOT (onCheckingFinished()));
}

/*!
 * Returns the downloaded change log as a \c QString.
 *
 * \sa setChangelogUrl()
 */

QString QSimpleUpdater::changeLog() const
{
    return m_changelog;
}

/*!
 * Checks for updates and calls the appropiate functions
 * when finished.
 *
 * \sa setDownloadUrl(), setReferenceUrl()
 */

void QSimpleUpdater::checkForUpdates (void)
{
    if (!m_reference_url.isEmpty())
    {
        m_manager->get (QNetworkRequest (m_reference_url));

        if (!silent())
            m_progressDialog->show();
    }

    else
        qDebug() << "QSimpleUpdater: Invalid reference URL";
}

/*!
 * Opens the download link in a web browser.
 *
 * \sa setDownloadUrl()
 */

void QSimpleUpdater::openDownloadLink (void)
{
    if (!m_download_url.isEmpty())
        QDesktopServices::openUrl (m_download_url);
}

/*!
 * Returns the latest version as a \c QString.
 *
 * \sa setReferenceUrl(), checkForUpdates()
 */

QString QSimpleUpdater::latestVersion() const
{
    return m_latest_version;
}

/*!
 * Returns the local version of the application.
 *
 * \sa setApplicationVersion()
 */

QString QSimpleUpdater::installedVersion() const
{
    return m_installed_version;
}

/*!
 * Downloads the latest version of the application
 * and displays download info in a dialog.
 *
 * \sa setDownloadUrl(), checkForUpdates()
 */

void QSimpleUpdater::downloadLatestVersion (void)
{
    if (!m_download_url.isEmpty())
        m_downloadDialog->beginDownload (m_download_url);
}

/*!
 * Returns \c true if the system detected that there is
 * a newer version of the application available online.
 *
 * \sa setReferenceUrl(), checkForUpdates()
 */

bool QSimpleUpdater::newerVersionAvailable() const
{
    return m_new_version_available;
}

/*!
 * Returns \c true if the system is set to
 * hide the progress dialog.
 *
 * \sa setSilent()
 */

bool QSimpleUpdater::silent() const
{
    return m_silent;
}

/*!
 * Tells the system from where to download the update packages
 * with the \a url parameter.
 *
 * Its recommended to use fixed URLs if you
 * want to automatically download and install your updates using
 * the bundled download dialog.
 *
 * \sa downloadLatestVersion(), openDownloadLink()
 */

void QSimpleUpdater::setDownloadUrl (const QString& url)
{
    Q_ASSERT (!url.isEmpty());
    m_download_url.setUrl (url);
}

/*!
 * Tells the updater system from where to download the file
 * that indicates the latest version using the \a url parameter.
 *
 * The reference file should contain \bold {ONLY} the latest version
 * in a plain text format. For example:
 *
 * \list
 * \o 1
 * \o 0.1
 * \o 1.0.1
 * \o 2.2.12
 * \o etc
 * \endlist
 *
 * \sa latestVersion()
 */

void QSimpleUpdater::setReferenceUrl (const QString& url)
{
    Q_ASSERT (!url.isEmpty());
    m_reference_url.setUrl (url);
}

/*!
 * Tells the updater system from where to download the
 * change log using the \a url parameter.
 * The change log can be any file you like, however,
 * its recommended to write it in plain text,
 * such as TXT, HTML and RTF files.
 *
 * \sa changeLog()
 */


void QSimpleUpdater::setChangelogUrl (const QString& url)
{
    Q_ASSERT (!url.isEmpty());
    m_changelog_url.setUrl (url);
}

/*!
 * Tells the updater the version of the installed
 * copy of your application using the \a version parameter.
 *
 * Calling this function is optional, as the default
 * values are loaded from QApplication class.
 *
 * \sa installedVersion(), checkForUpdates()
 */


void QSimpleUpdater::setApplicationVersion (const QString& version)
{
    Q_ASSERT (!version.isEmpty());
    m_installed_version = version;
}

/*!
 *
 * \list
 * \o If \a silent is set to \c true, no dialogs will be shown while checking
 * for updates or when a newer version of the application is found.
 * \o If \a silent is set to \c false, a dialog will be shown while checking for
 * updates or when a newer version of the application is found.
 * \endlist
 *
 * \sa silent()
 */

void QSimpleUpdater::setSilent (bool silent)
{
    m_silent = silent;

    if (m_silent)
        setShowNewestVersionMessage (false);
}

/*!
 * If the \a show parameter is set to \c true, the updater system will show a
 * message box notifying the user when there's an update available.
 *
 * \sa checkForUpdates()
 */

void QSimpleUpdater::setShowUpdateAvailableMessage (bool show)
{
    m_show_update_available = show;
}

/*!
 * If the \a show parameter is set to \c true, the updater system will show a
 * message box notifying the user when the latest version is already installed.
 *
 * For example, this configuration is useful when the user manually calls this function.
 * \i {Eg:} when he or she clicks the "Check for updates" menu item.
 *
 * \sa checkForUpdates()
 */

void QSimpleUpdater::setShowNewestVersionMessage (bool show)
{
    m_show_newest_version = show;
}

/*! \internal
 * Disconnects the network access manager when the user
 * clicks on the "cancel" button in the progress dialog.
 */

void QSimpleUpdater::cancel (void)
{
    m_manager->disconnect();
}

/*! \internal
 * Alerts the user when the download of version information
 * data is corrupted.
 *
 * \sa checkDownloadedVersion()
 */

void QSimpleUpdater::showErrorMessage (void)
{
    if (!silent())
    {
        m_progressDialog->hide();
        QMessageBox::warning (NULL, tr ("Software Updater"),
                              tr ("An unknown error occured while checking for updates"));
    }
}

/*! \internal
 * Informs the user if there's a newer version available for download
 * or if he or she is running the latest version of the application.
 *
 * \sa checkDownloadedVersion()
 */

void QSimpleUpdater::onCheckingFinished (void)
{
    // Hide the progress dialog
    m_progressDialog->hide();

    // Get the application icon as a pixmap
    QPixmap _icon = qApp->windowIcon().pixmap (
                        qApp->windowIcon().actualSize (QSize (96, 96)));

    // If the icon is invalid, use default icon
    if (_icon.isNull())
        _icon = QPixmap (":/icons/update.png");

    QMessageBox _message;
    _message.setIconPixmap (_icon);

    // Ask user if he/she wants to download newer version
    if (newerVersionAvailable() && m_show_update_available)
    {
        _message.setDetailedText (changeLog());
        _message.setStandardButtons (QMessageBox::Yes | QMessageBox::No);
        _message.setText ("<b>" + tr ("A new version of %1 is available!").arg (qApp->applicationName()) + "</b>");
        _message.setInformativeText (tr ("%1 %2 is available - you have %3. Would you like to download it now?")
                                     .arg (qApp->applicationName())
                                     .arg (latestVersion())
                                     .arg (installedVersion()));

        if (_message.exec() == QMessageBox::Yes)
            downloadLatestVersion();
    }

    // Tell user that he/she is up to date (only if necessary)
    else if (!silent() && m_show_newest_version && !m_latest_version.isEmpty())
    {
        _message.setStandardButtons (QMessageBox::Ok);
        _message.setText ("<b>" + tr ("You're up-to-date!") +
                          "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</b>");
        _message.setInformativeText (
            tr ("%1 %2 is currently the newest version available")
            .arg (qApp->applicationName())
            .arg (installedVersion()));

        _message.exec();
    }
}

/*! \internal
 * Compares downloaded version information with local version information
 * and decides if there's a newer version available.
 *
 * Finally, the function downloads the changelog if there's a newer version
 * available online.
 *
 * \sa checkForUpdates()
 */

void QSimpleUpdater::checkDownloadedVersion (QNetworkReply *reply)
{
    bool _new_update = false;

    QString _reply = QString::fromUtf8 (reply->readAll());
    _reply.replace (" ", "");
    _reply.replace ("\n", "");

    if (!_reply.isEmpty())
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

    else
        showErrorMessage();

    m_new_version_available = _new_update;

    if (!m_changelog_url.isEmpty() && newerVersionAvailable())
    {
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

/*! \internal
 * Reads the downloaded changelog data and transforms it into a QString.
 *
 * \sa setChangelogUrl(), changeLog()
 */

void QSimpleUpdater::processDownloadedChangelog (QNetworkReply *reply)
{
    QString _reply = QString::fromUtf8 (reply->readAll());

    if (!_reply.isEmpty())
        m_changelog = _reply;

    emit checkingFinished();
}

/*! \internal
 * Allows the download process to continue even if there are SSL errors.
 *
 * \sa checkForUpdates()
 */

void QSimpleUpdater::ignoreSslErrors (QNetworkReply *reply,
                                      const QList<QSslError>& error)
{
#if SUPPORTS_SSL
    reply->ignoreSslErrors (error);
#else
    Q_UNUSED (reply);
    Q_UNUSED (error);
#endif
}
