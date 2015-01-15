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
 *
 */

#include "download_dialog.h"
#include "ui_download_dialog.h"

DownloadDialog::DownloadDialog (QWidget *parent)
    : QWidget (parent)
    , ui (new Ui::DownloadDialog) {

    // Setup the UI
    ui->setupUi (this);

    // Make the window look like a dialog
    QIcon _blank;
    setWindowIcon (_blank);
    setWindowModality (Qt::WindowModal);
    setWindowFlags (Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);

    // Connect SIGNALS/SLOTS
    connect (ui->stopButton, SIGNAL (clicked()), this, SLOT (cancelDownload()));
    connect (ui->openButton, SIGNAL (clicked()), this, SLOT (installUpdate()));

    // Configure open button
    ui->openButton->setEnabled (false);
    ui->openButton->setVisible (false);

    // Initialize the network access manager
    m_manager = new QNetworkAccessManager (this);

    // Avoid SSL issues
    connect (m_manager, SIGNAL (sslErrors (QNetworkReply *, QList<QSslError>)), this,
             SLOT (ignoreSslErrors (QNetworkReply *, QList<QSslError>)));
}

DownloadDialog::~DownloadDialog (void) {
    delete ui;
}

void DownloadDialog::beginDownload (const QUrl& url) {
    Q_ASSERT (!url.isEmpty());

    // Reset the UI
    ui->progressBar->setValue (0);
    ui->stopButton->setText (tr ("Stop"));
    ui->downloadLabel->setText (tr ("Downloading updates"));
    ui->timeLabel->setText (tr ("Time remaining") + ": " + tr ("unknown"));

    // Begin the download
    m_reply = m_manager->get (QNetworkRequest (url));
    m_start_time = QDateTime::currentDateTime().toTime_t();

    // Update the progress bar value automatically
    connect (m_reply, SIGNAL (downloadProgress (qint64, qint64)), this,
             SLOT (updateProgress (qint64, qint64)));

    // Write the file to the hard disk once the download is finished
    connect (m_reply, SIGNAL (finished()), this, SLOT (downloadFinished()));

    // Show the dialog
    showNormal();
}

void DownloadDialog::installUpdate (void) {
    QMessageBox msg;
    msg.setIcon (QMessageBox::Question);
    msg.setText ("<b>" +
                 tr ("To apply the update(s), you must first quit %1")
                 .arg (qApp->applicationName()) +
                 "</b>");
    msg.setInformativeText (tr ("Do you want to quit %1 now?").arg (qApp->applicationName()));
    msg.setStandardButtons (QMessageBox::Yes | QMessageBox::No);

    if (msg.exec() == QMessageBox::Yes) {
        openDownload();
        qApp->closeAllWindows();
    }

    else {
        ui->openButton->setEnabled (true);
        ui->openButton->setVisible (true);
        ui->timeLabel->setText (tr ("Click the \"Open\" button to apply the update"));
    }
}

void DownloadDialog::openDownload (void) {
    if (!m_path.isEmpty()) {
        QString url = m_path;

        if (url.startsWith ("/"))
            url = "file://" + url;

        else
            url = "file:///" + url;

        QDesktopServices::openUrl (url);
    }
}

void DownloadDialog::cancelDownload (void) {
    if (!m_reply->isFinished()) {
        QMessageBox _message;
        _message.setWindowTitle (tr ("Updater"));
        _message.setIcon (QMessageBox::Question);
        _message.setStandardButtons (QMessageBox::Yes | QMessageBox::No);
        _message.setText (tr ("Are you sure you want to cancel the download?"));

        if (_message.exec() == QMessageBox::Yes) {
            hide();
            m_reply->abort();
        }
    }

    else
        hide();
}

void DownloadDialog::downloadFinished (void) {
    ui->stopButton->setText (tr ("Close"));
    ui->downloadLabel->setText (tr ("Download complete!"));
    ui->timeLabel->setText (tr ("The installer will open in a separate window..."));

    QByteArray data = m_reply->readAll();

    if (!data.isEmpty()) {
        QStringList list = m_reply->url().toString().split ("/");
        QFile file (QDir::tempPath() + "/" + list.at (list.count() - 1));

        if (file.open (QIODevice::WriteOnly)) {
            file.write (data);
            m_path = file.fileName();
        }

        file.close();
        installUpdate();
    }
}

void DownloadDialog::updateProgress (qint64 received, qint64 total) {
    // We know the size of the download, so we can calculate the progress....
    if (total > 0 && received > 0) {
        ui->progressBar->setMinimum (0);
        ui->progressBar->setMaximum (100);

        int _progress = (int) ((received * 100) / total);
        ui->progressBar->setValue (_progress);

        QString _total_string;
        QString _received_string;

        float _total = total;
        float _received = received;

        if (_total < 1024)
            _total_string = tr ("%1 bytes").arg (_total);

        else if (_total < 1024 * 1024) {
            _total = roundNumber (_total / 1024);
            _total_string = tr ("%1 KB").arg (_total);
        }

        else {
            _total = roundNumber (_total / (1024 * 1024));
            _total_string = tr ("%1 MB").arg (_total);
        }

        if (_received < 1024)
            _received_string = tr ("%1 bytes").arg (_received);

        else if (received < 1024 * 1024) {
            _received = roundNumber (_received / 1024);
            _received_string = tr ("%1 KB").arg (_received);
        }

        else {
            _received = roundNumber (_received / (1024 * 1024));
            _received_string = tr ("%1 MB").arg (_received);
        }

        ui->downloadLabel->setText (tr ("Downloading updates") + " (" + _received_string + " " + tr ("of") + " " + _total_string + ")");

        uint _diff = QDateTime::currentDateTime().toTime_t() - m_start_time;

        if (_diff > 0) {
            QString _time_string;
            float _time_remaining = total / (received / _diff);

            if (_time_remaining > 7200) {
                _time_remaining /= 3600;
                _time_string = tr ("About %1 hours").arg (int (_time_remaining + 0.5));
            }

            else if (_time_remaining > 60) {
                _time_remaining /= 60;
                _time_string = tr ("About %1 minutes").arg (int (_time_remaining + 0.5));
            }

            else if (_time_remaining <= 60)
                _time_string = tr ("%1 seconds").arg (int (_time_remaining + 0.5));

            ui->timeLabel->setText (tr ("Time remaining") + ": " + _time_string);
        }
    }

    // We do not know the size of the download, so we avoid scaring the shit out
    // of the user
    else {
        ui->progressBar->setValue (-1);
        ui->progressBar->setMinimum (0);
        ui->progressBar->setMaximum (0);
        ui->downloadLabel->setText (tr ("Downloading updates"));
        ui->timeLabel->setText (tr ("Time remaining") + ": " + tr ("Unknown"));
    }
}

void DownloadDialog::ignoreSslErrors (QNetworkReply *reply,
                                      const QList<QSslError>& error) {
#ifndef Q_OS_IOS
    reply->ignoreSslErrors (error);
#else
    Q_UNUSED (reply);
    Q_UNUSED (error);
#endif
}

float DownloadDialog::roundNumber (const float& input) {
    return roundf (input * 100) / 100;
}
