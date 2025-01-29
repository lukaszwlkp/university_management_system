#include "changepassworddialog.h"
#include "ui_changepassworddialog.h"

ChangePasswordDialog::ChangePasswordDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangePasswordDialog)
{
    ui->setupUi(this);
    setWindowTitle("Change Password");
    QWidget::setTabOrder(ui->lineEdit_oldPassword, ui->lineEdit_newPassword);
}

ChangePasswordDialog::~ChangePasswordDialog()
{
    delete ui;
}

QString ChangePasswordDialog::getOldPassword() const
{
    return ui->lineEdit_oldPassword->text();
}

QString ChangePasswordDialog::getNewPassword() const
{
    return ui->lineEdit_newPassword->text();
}
