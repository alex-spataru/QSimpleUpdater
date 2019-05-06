#include "IDownloader.h"
#include <QDateTime>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QApplication>

const QString IDownloader::PARTIAL_DOWN(".part");

/**
 * Returns \c true if the updater shall not intervene when the download has
 * finished (you can use the \c QSimpleUpdater signals to know when the
 * download is completed).
 */
IDownloader::IDownloader()
{
    /* Initialize private members */
    m_manager = new QNetworkAccessManager();
    /* Initialize internal values */
    m_url = "";
    m_fileName = "";
    m_startTime = 0;
    m_useCustomProcedures = false;
    m_mandatoryUpdate = false;
    /* Set download directory */
    m_downloadDir = QDir::homePath() + "/Downloads/";
}

IDownloader::~IDownloader()
{
    delete m_reply;
    delete m_manager;
}

bool IDownloader::useCustomInstallProcedures() const
{
    return m_useCustomProcedures;
}

/**
 * Changes the URL, which is used to indentify the downloader dialog
 * with an \c Updater instance
 *
 * \note the \a url parameter is not the download URL, it is the URL of
 *       the AppCast file
 */
void IDownloader::setUrlId (const QString& url)
{
    m_url = url;
}

/**
 * Changes the name of the downloaded file
 */
void IDownloader::setFileName (const QString& file)
{
    m_fileName = file;

    if (m_fileName.isEmpty())
        m_fileName = "QSU_Update.bin";
}

/**
 * Changes the user-agent string used to communicate with the remote HTTP server
 */
void IDownloader::setUserAgentString (const QString& agent)
{
    m_userAgentString = agent;
}


QString IDownloader::downloadDir() const
{
    return m_downloadDir.absolutePath();
}

void IDownloader::setDownloadDir (const QString& downloadDir)
{
    if (m_downloadDir.absolutePath() != downloadDir)
        m_downloadDir = downloadDir;
}

/**
 * If the \a mandatory_update is set to \c true, the \c Downloader has to download and install the
 * update. If the user cancels or exits, the application will close
 */
void IDownloader::setMandatoryUpdate(const bool mandatory_update)
{
    m_mandatoryUpdate = mandatory_update;
}

/**
 * If the \a custom parameter is set to \c true, then the \c Downloader will not
 * attempt to open the downloaded file.
 *
 * Use the signals fired by the \c QSimpleUpdater to implement your own install
 * procedures.
 */
void IDownloader::setUseCustomInstallProcedures (const bool custom)
{
    m_useCustomProcedures = custom;
}

void IDownloader::startDownload(const QUrl& url)
{
    /* Configure the network request */
    QNetworkRequest request (url);
    if (!m_userAgentString.isEmpty())
        request.setRawHeader ("User-Agent", m_userAgentString.toUtf8());

    /* Start download */
    m_reply = m_manager->get (request);
    m_startTime = QDateTime::currentDateTime().toTime_t();

    /* Ensure that downloads directory exists */
    if (!m_downloadDir.exists())
        m_downloadDir.mkpath (".");

    /* Remove old downloads */
    QFile::remove (m_downloadDir.filePath (m_fileName));
    QFile::remove (m_downloadDir.filePath (m_fileName + PARTIAL_DOWN));
}

void IDownloader::finished()
{
    /* Rename file */
    QFile::rename (m_downloadDir.filePath (m_fileName + PARTIAL_DOWN),
                   m_downloadDir.filePath (m_fileName));

    /* Notify application */
    emit downloadFinished (m_url, m_downloadDir.filePath (m_fileName));

    m_reply->close();

    /* Install the update */
    installUpdate();
}

void IDownloader::cancelDownload()
{
    if (!m_reply->isFinished())
    {
        m_reply->abort();
    }

    if(m_mandatoryUpdate)
    {
        QApplication::quit();
    }
}

void IDownloader::installUpdate()
{
    if (useCustomInstallProcedures())
    {
        return;
    }
    else
    {
        openDownload();
    }
}

/**
 * Opens the downloaded file.
 * \note If the downloaded file is not found, then the function will alert the
 *       user about the error.
 */
void IDownloader::openDownload()
{
    if (!m_fileName.isEmpty())
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(m_downloadDir.filePath (m_fileName)));
    }
}

/**
 * Writes the downloaded data to the disk
 */
void IDownloader::saveFile(qint64 /*received*/, qint64 /*total*/)
{
    /* Check if we need to redirect */
    QUrl url = m_reply->attribute (QNetworkRequest::RedirectionTargetAttribute).toUrl();
    if (!url.isEmpty())
    {
        startDownload(url);
        return;
    }

    /* Save downloaded data to disk */
    QFile file (m_downloadDir.filePath (m_fileName + PARTIAL_DOWN));
    if (file.open (QIODevice::WriteOnly | QIODevice::Append)) {
        file.write (m_reply->readAll());
        file.close();
    }
}
