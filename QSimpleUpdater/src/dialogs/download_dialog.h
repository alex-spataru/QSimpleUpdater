#ifndef DOWNLOAD_DIALOG_H
#define DOWNLOAD_DIALOG_H

#include <QDir>
#include <QIcon>
#include <QDebug>
#include <QWidget>
#include <QDateTime>
#include <QMessageBox>
#include <QNetworkReply>
#include <QDesktopServices>
#include <QNetworkAccessManager>

#include <math.h>

namespace Ui {
class DownloadDialog;
}

class DownloadDialog : public QWidget {
    Q_OBJECT

public:
    explicit DownloadDialog (QWidget *parent = 0);
    ~DownloadDialog(void);

    void beginDownload (const QUrl& url);

private slots:
    void openDownload(void);
    void cancelDownload(void);
    void downloadFinished(void);
    void updateProgress (qint64 received, qint64 total);
    void ignoreSslErrors (QNetworkReply *reply, const QList<QSslError>& error);

private:
    Ui::DownloadDialog *ui;

    QString m_path;
    bool m_download_paused;

    QNetworkReply *m_reply;
    QNetworkAccessManager *m_manager;

    uint m_start_time;

    float roundNumber (const float& input);
};

#endif
