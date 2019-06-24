/*
 * Copyright (c) 2014-2016 Alex Spataru <alex_spataru@outlook.com>
 * Copyright (c) 2017 Gilmanov Ildar <https://github.com/gilmanov-ildar>
 *
 * This file is part of the QSimpleUpdater library, which is released under
 * the DBAD license, you can read a copy of it below:
 *
 * DON'T BE A DICK PUBLIC LICENSE TERMS AND CONDITIONS FOR COPYING,
 * DISTRIBUTION AND MODIFICATION:
 *
 * Do whatever you like with the original work, just don't be a dick.
 * Being a dick includes - but is not limited to - the following instances:
 *
 * 1a. Outright copyright infringement - Don't just copy this and change the
 *     name.
 * 1b. Selling the unmodified original with no work done what-so-ever, that's
 *     REALLY being a dick.
 * 1c. Modifying the original work to contain hidden harmful content.
 *     That would make you a PROPER dick.
 *
 * If you become rich through modifications, related works/services, or
 * supporting the original work, share the love.
 * Only a dick would make loads off this work and not buy the original works
 * creator(s) a pint.
 *
 * Code is provided with no warranty. Using somebody else's code and bitching
 * when it goes wrong makes you a DONKEY dick.
 * Fix the problem yourself. A non-dick would submit the fix back.
 */

#ifndef DOWNLOADERLITE_H
#define DOWNLOADERLITE_H

#include <QObject>
#include "IDownloader.h"

class DownloaderLite : public QObject, public IDownloader
{
    Q_OBJECT
    Q_INTERFACES(IDownloader)

public:
    explicit DownloaderLite(QObject *parent = nullptr) : QObject(parent){}
    virtual ~DownloaderLite() {}

signals:
    void downloadFinished (const QString& url, const QString& filepath) override;

public slots:
    virtual void startDownload (const QUrl& url) override {IDownloader::startDownload(url);}

protected slots:
    virtual void finished() override {IDownloader::finished();}
    virtual void cancelDownload() override{IDownloader::cancelDownload();}
    virtual void installUpdate() override{IDownloader::installUpdate();}
    virtual void openDownload() override{IDownloader::openDownload();}
    virtual void saveFile(qint64 received, qint64 total) override{IDownloader::saveFile(received,total);}
    virtual void updateProgress(qint64 received, qint64 total) override{IDownloader::updateProgress(received,total);}

protected:
    virtual const QObject* getThisQObj() override {return this;}
};

#endif // DOWNLOADERLITE_H
