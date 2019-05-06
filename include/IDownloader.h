#ifndef DOWNLOADERINTERFACE_H
#define DOWNLOADERINTERFACE_H

#include <QString>
#include <QDir>

class QNetworkReply;
class QNetworkAccessManager;

class IDownloader
{
public:
    IDownloader();
    virtual ~IDownloader();

    bool useCustomInstallProcedures() const;

    QString downloadDir() const;
    void setDownloadDir (const QString& downloadDir);

signals:
    virtual void downloadFinished (const QString& url, const QString& filepath) = 0;

public slots:
    void setUrlId (const QString& url);
    void setFileName (const QString& file);
    void setUserAgentString (const QString& agent);
    void setUseCustomInstallProcedures (const bool custom);
    void setMandatoryUpdate (const bool mandatory_update);

    virtual void startDownload (const QUrl& url);

protected slots:
    virtual void finished();
    virtual void cancelDownload();
    virtual void installUpdate();
    virtual void openDownload();
    virtual void saveFile(qint64 received, qint64 total);

    QString m_url;
    uint m_startTime;
    QDir m_downloadDir;
    QString m_fileName;
    QNetworkReply* m_reply;
    QString m_userAgentString;

    bool m_useCustomProcedures;
    bool m_mandatoryUpdate;

    QNetworkAccessManager* m_manager;

    static const QString PARTIAL_DOWN;
};

Q_DECLARE_INTERFACE(IDownloader, "IDownloader")

#endif // DOWNLOADERINTERFACE_H
