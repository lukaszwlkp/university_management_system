#include "manageusersdialog.h"
#include "ui_manageusersdialog.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

extern DatabaseManager dbManager;

ManageUsersDialog::ManageUsersDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ManageUsersDialog)
{
    ui->setupUi(this);
    setWindowTitle("Manage Users");

    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &ManageUsersDialog::onSave);
}

ManageUsersDialog::~ManageUsersDialog()
{
    delete ui;
}

void ManageUsersDialog::setUserData(const QString &id, const QString &firstName, const QString &secondName,
                                   const QString &email, const QString &additionalInfo)
{
    ui->lineEdit_id->setText(id);
    ui->lineEdit_firstName->setText(firstName);
    ui->lineEdit_secondName->setText(secondName);
    ui->lineEdit_email->setText(email);
    ui->lineEdit_additionalInfo->setText(additionalInfo);
}

void ManageUsersDialog::onSave()
{
    QString newId = ui->lineEdit_id->text().trimmed();
    QString newEmail = ui->lineEdit_email->text().trimmed();

    if (isIdAlreadyUsed(newId)) {
        QMessageBox::warning(this, "Duplicate ID", "The user ID is already in use.");
        return;
    }

    if (isEmailAlreadyUsed(newEmail)) {
        QMessageBox::warning(this, "Duplicate Email", "The email address is already in use.");
        return;
    }

    QSqlQuery query(dbManager.getDatabase());
    query.prepare("UPDATE users SET id = :id, firstName = :firstName, secondName = :secondName, "
                  "email = :email, degree = :degree, fieldOfStudy = :fieldOfStudy, "
                  "semester = :semester, groupNumber = :groupNumber WHERE id = :currentId");

    query.bindValue(":id", newId);
    query.bindValue(":firstName", ui->lineEdit_firstName->text().trimmed());
    query.bindValue(":secondName", ui->lineEdit_secondName->text().trimmed());
    query.bindValue(":email", newEmail);

    if (newId.startsWith("1")) {
        QStringList additionalInfo = ui->lineEdit_additionalInfo->text().split(", ");
        query.bindValue(":fieldOfStudy", additionalInfo[0].trimmed());
        query.bindValue(":semester", additionalInfo[1].mid(4).trimmed()); // sem.
        query.bindValue(":groupNumber", additionalInfo[2].mid(3).trimmed()); // gr.
    } else {
        query.bindValue(":degree", ui->lineEdit_additionalInfo->text().trimmed());
    }

    query.bindValue(":currentId", newId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to update user: " + query.lastError().text());
        return;
    }

    accept();
}

bool ManageUsersDialog::isEmailAlreadyUsed(const QString& email)
{
    QString newId = ui->lineEdit_id->text().trimmed();
    QSqlQuery query(dbManager.getDatabase());
    query.prepare("SELECT COUNT(*) FROM users WHERE email = :email AND id != :id");
    query.bindValue(":email", email);
    query.bindValue(":id", newId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to check email: " + query.lastError().text());
        return false;
    }

    query.next();
    return query.value(0).toInt() > 0;
}

bool ManageUsersDialog::isIdAlreadyUsed(const QString& id)
{
    QSqlQuery query(dbManager.getDatabase());
    query.prepare("SELECT COUNT(*) FROM users WHERE id = :id AND id != :currentId");
    query.bindValue(":id", id);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to check ID: " + query.lastError().text());
        return false;
    }

    query.next();
    return query.value(0).toInt() > 0;
}
