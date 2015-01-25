#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <QDialog>
#include <QMessageBox>
#include <QSimpleUpdater>

namespace Ui
{
class Example;
}

class Example : public QDialog
{
        Q_OBJECT

    public:
        explicit Example (QWidget *parent = 0);
        ~Example();

    public slots:
        void checkForUpdates();
        void onCheckingFinished();

    private:
        Ui::Example *ui;

        QString m_installed_version;
        QSimpleUpdater *updater;
};

#endif
