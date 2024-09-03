#include "AuthenticateDialog.h"
#include "ui_AuthenticateDialog.h"

AuthenticateDialog::AuthenticateDialog(QWidget *parent)
   : QDialog(parent)
   , ui(new Ui::AuthenticateDialog)
{
   ui->setupUi(this);
}

AuthenticateDialog::~AuthenticateDialog()
{
   delete ui;
}

void AuthenticateDialog::setUserName(const QString &userName)
{
   ui->userLE->setText(userName);
}

void AuthenticateDialog::setPassword(const QString &password)
{
   ui->passwordLE->setText(password);
}

QString AuthenticateDialog::userName() const
{
   return ui->userLE->text();
}

QString AuthenticateDialog::password() const
{
   return ui->passwordLE->text();
}
