#ifndef DOWNLOADERLITE_H
#define DOWNLOADERLITE_H

#include <QObject>
#include "IDownloader.h"

class DownloaderLite : public QObject, public IDownloader
{
    Q_OBJECT
    Q_INTERFACES(IDownloader)

public:
    explicit DownloaderLite(QObject *parent = nullptr);
    virtual ~DownloaderLite() {}

signals:
    void downloadFinished (const QString& url, const QString& filepath) override;

public slots:
    virtual void startDownload (const QUrl& url) override;

protected slots:
    virtual void finished() override {IDownloader::finished();}
    virtual void cancelDownload() override{IDownloader::cancelDownload();}
    virtual void installUpdate() override{IDownloader::installUpdate();}
    virtual void openDownload() override{IDownloader::openDownload();}
    virtual void saveFile(qint64 received, qint64 total) override{IDownloader::saveFile(received,total);}
};

#endif // DOWNLOADERLITE_H
