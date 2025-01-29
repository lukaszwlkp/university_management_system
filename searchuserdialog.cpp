#include "searchuserdialog.h"
#include "ui_searchuserdialog.h"

SearchUserDialog::SearchUserDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SearchUserDialog)
{
    ui->setupUi(this);
    setWindowTitle("Search User");
}

SearchUserDialog::~SearchUserDialog()
{
    delete ui;
}

void SearchUserDialog::setUserData(const QString &id, const QString &firstName, const QString &secondName,
                                   const QString &email, const QString &additionalInfo)
{
    ui->lineEdit_id->setText(id);
    ui->lineEdit_firstName->setText(firstName);
    ui->lineEdit_secondName->setText(secondName);
    ui->lineEdit_email->setText(email);
    ui->lineEdit_additionalInfo->setText(additionalInfo);
}
