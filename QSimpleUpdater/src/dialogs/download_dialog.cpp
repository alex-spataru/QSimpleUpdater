//
//  This file is part of QSimpleUpdater
//
//  Copyright (c) 2014 Alex Spataru <alex_spataru@gmail.com>
//
//  Please check the license.txt file for more information.
//

#include "download_dialog.h"
#include "ui_download_dialog.h"

DownloadDialog::DownloadDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadDialog)
{
    // Setup the UI
    ui->setupUi(this);
    ui->installButton->setEnabled(false);

    // Connect SIGNALS/SLOTS
    connect(ui->installButton, SIGNAL(clicked()), this, SLOT(openDownload()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancelDownload()));

    // Initialize the network access manager
    m_manager = new QNetworkAccessManager(this);

    // Avoid SSL issues
    connect(m_manager, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)),
            this, SLOT(ignoreSslErrors(QNetworkReply*,QList<QSslError>)));
}

DownloadDialog::~DownloadDialog()
{
    delete ui;
}

void DownloadDialog::beginDownload(const QUrl &url)
{
    Q_ASSERT(!url.isEmpty());

    // Reset the UI
    ui->progressBar->setValue(0);
    ui->installButton->setEnabled(false);
    ui->cancelButton->setText(tr("Cancel"));
    ui->downloadLabel->setText(tr("Downloading updates"));
    ui->timeLabel->setText(tr("Time remaining") + ": " + tr("unknown"));

    // Begin the download
    m_reply = m_manager->get(QNetworkRequest(url));
    m_start_time = QDateTime::currentDateTime().toTime_t();

    // Update the progress bar value automatically
    connect(m_reply, SIGNAL(downloadProgress(qint64,qint64)),
            this, SLOT(updateProgress(qint64,qint64)));

    // Write the file to the hard disk once the download is finished
    connect(m_reply, SIGNAL(finished()), this, SLOT(downloadFinished()));

    // Show the dialog
    showNormal();
}

void DownloadDialog::openDownload()
{
    if (!m_path.isEmpty()) {
        QString url = m_path;

        // Build a correct URL to open local files
        if (url.startsWith("/"))
            url = "file://" + url;
        else
            url = "file:///" + url;

        // Let the system open the downloaded file
        QDesktopServices::openUrl(url);
    }

    else {
        qWarning() << "QSimpleUpdater: cannot open downloaded file!";
    }
}

void DownloadDialog::cancelDownload()
{
    QMessageBox _message;
    _message.setWindowTitle(tr("Updater"));
    _message.setIcon(QMessageBox::Question);
    _message.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    _message.setText(tr("Are you sure you want to cancel the download?"));

    if (_message.exec() == QMessageBox::Yes) {
        hide();
        m_reply->abort();
    }
}

void DownloadDialog::downloadFinished()
{
    ui->timeLabel->setText(NULL);
    ui->installButton->setEnabled(true);
    ui->cancelButton->setText(tr("Close"));
    ui->downloadLabel->setText(tr("Download complete!"));

    QByteArray data = m_reply->readAll();

    if (!data.isEmpty()) {
        // Create a QFile with the name of the downloaded file
        QStringList list = m_reply->url().toString().split("/");
        QFile file(QDir::tempPath() + "/" + list.at(list.count() - 1));

        if (file.open(QIODevice::WriteOnly)) {
            file.write(data);
            m_path = file.fileName();
        }

        else {
            qWarning() << "QSimpleUpdater: cannot write downloaded data!";
        }

        file.close();
    }

    else {
        qWarning() << "QSimpleUpdater: invalid download data!";
    }
}

void DownloadDialog::updateProgress(qint64 received, qint64 total)
{
    // We know the size of the download, so we can calculate the progress....
    if (total > 0 && received > 0) {
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(100);

        int _progress = (int) ((received * 100) / total);
        ui->progressBar->setValue(_progress);

        QString _total_string;
        QString _received_string;

        float _total = total;
        float _received = received;

        if (_total < 1024) {
            _total_string = tr("%1 bytes").arg(_total);
        } else if (_total < 1024 * 1024) {
            _total = roundNumber(_total / 1024);
            _total_string = tr("%1 KB").arg(_total);
        } else {
            _total = roundNumber(_total / (1024 * 1024));
            _total_string = tr("%1 MB").arg(_total);
        }

        if (_received < 1024) {
            _received_string = tr("%1 bytes").arg(_received);
        } else if (received < 1024 * 1024) {
            _received = roundNumber(_received / 1024);
            _received_string = tr("%1 KB").arg(_received);
        } else {
            _received = roundNumber(_received / (1024 * 1024));
            _received_string = tr("%1 MB").arg(_received);
        }

        ui->downloadLabel->setText(tr("Downloading updates") + " (" +
                                   _received_string + " " + tr("of") + " " +
                                   _total_string + ")");

        uint _diff = QDateTime::currentDateTime().toTime_t() - m_start_time;

        if (_diff > 0) {
            QString _time_string;
            float _time_remaining = total / (received / _diff);

            if (_time_remaining > 7200) {
                _time_remaining /= 3600;
                _time_string = tr("About %1 hours").arg(int(_time_remaining + 0.5));
            }

            else if (_time_remaining > 60) {
                _time_remaining /= 60;
                _time_string = tr("About %1 minutes").arg(int(_time_remaining + 0.5));
            }

            else if (_time_remaining <= 60) {
                _time_string = tr("%1 seconds").arg(int(_time_remaining + 0.5));
            }

            ui->timeLabel->setText(tr("Time remaining") + ": " + _time_string);
        }
    }

    // We do not know the size of the download, so we improvise...
    else {
        ui->progressBar->setValue(-1);
        ui->progressBar->setMinimum(0);
        ui->progressBar->setMaximum(0);
        ui->downloadLabel->setText(tr("Downloading updates"));
        ui->timeLabel->setText(tr("Time remaining") + ": " + tr("Unknown"));
    }
}

void DownloadDialog::ignoreSslErrors (QNetworkReply *reply, const QList<QSslError> &error)
{
    reply->ignoreSslErrors (error);
}

float DownloadDialog::roundNumber(const float &input)
{
    return roundf(input * 100) / 100;
}
