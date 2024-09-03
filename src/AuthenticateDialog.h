#ifndef AUTHENTICATEDIALOG_H
#define AUTHENTICATEDIALOG_H

#include <QDialog>

namespace Ui
{
class AuthenticateDialog;
}

class AuthenticateDialog : public QDialog
{
   Q_OBJECT

public:
   explicit AuthenticateDialog(QWidget *parent = nullptr);
   ~AuthenticateDialog();

   void setUserName(const QString &userName);
   void setPassword(const QString &password);
   QString userName() const;
   QString password() const;

private:
   Ui::AuthenticateDialog *ui;
};

#endif // AUTHENTICATEDIALOG_H
