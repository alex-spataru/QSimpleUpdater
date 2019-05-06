#include "DownloaderLite.h"
#include <QNetworkReply>
#include <QDesktopServices>

DownloaderLite::DownloaderLite(QObject *parent) : QObject(parent)
{
}

void DownloaderLite::startDownload(const QUrl& url)
{
    IDownloader::startDownload(url);

    connect(m_reply, SIGNAL(finished()), this, SLOT(finished()));
    connect(m_reply, SIGNAL(redirected(QUrl)), this, SLOT(startDownload(QUrl)));
}
