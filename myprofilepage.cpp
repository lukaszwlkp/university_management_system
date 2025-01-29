#include "myprofilepage.h"
#include "ui_myprofilepage.h"
#include "databasemanager.h"
#include "changepassworddialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

extern DatabaseManager dbManager;

MyProfilePage::MyProfilePage(const QString& loggedUserId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyProfilePage),
    loggedUserId(loggedUserId)
{
    ui->setupUi(this);

    if (!dbManager.getDatabase().isOpen()) {
        if (!dbManager.connectDatabase()) {
            QMessageBox::critical(this, "Database Error", "Failed to connect to the database.");
            return;
        }
    }

    QSqlQuery query(dbManager.getDatabase());

    // Sprawdzanie, czy ID użytkownika zaczyna się na '1' (student)
    bool isStudent = loggedUserId.startsWith("1");

    // Jeśli to student, pobieramy fieldOfStudy, semester i groupNumber, w przeciwnym razie degree
    if (isStudent) {
        query.prepare("SELECT fieldOfStudy, semester, groupNumber FROM users WHERE id = :id");
    } else {
        query.prepare("SELECT password, firstName, secondName, email, degree FROM users WHERE id = :id");
    }

    query.bindValue(":id", loggedUserId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Query execution failed: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        ui->lineEdit_id->setText(loggedUserId);
        ui->lineEdit_firstName->setText(query.value("firstName").toString());
        ui->lineEdit_secondName->setText(query.value("secondName").toString());
        ui->lineEdit_email->setText(query.value("email").toString());

        if (isStudent) {
            // Dla studenta: fieldOfStudy, semester, groupNumber
            QString studentInfo = query.value("fieldOfStudy").toString() + ", sem. " +
                                  query.value("semester").toString() + ", gr. " +
                                  query.value("groupNumber").toString();
            ui->lineEdit_additionalInfo->setText(studentInfo);
        } else {
            ui->lineEdit_additionalInfo->setText(query.value("degree").toString());
        }
    } else {
        QMessageBox::warning(this, "Profile", "No data found for the given user ID.");
    }
}

MyProfilePage::~MyProfilePage()
{
    delete ui;
}

void MyProfilePage::on_pushButton_return_clicked()
{
    emit backToMenu();
}

void MyProfilePage::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        on_pushButton_return_clicked();
    }
}

void MyProfilePage::on_pushButton_changePassword_clicked()
{
    ChangePasswordDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        QString oldPassword = dialog.getOldPassword();
        QString newPassword = dialog.getNewPassword();

        if (oldPassword.isEmpty() || newPassword.isEmpty()) {
            QMessageBox::warning(this, "Change Password", "Please enter both old and new passwords.");
            return;
        }

        QSqlQuery query(dbManager.getDatabase());
        query.prepare("SELECT password FROM users WHERE id = :id");
        query.bindValue(":id", loggedUserId);

        if (!query.exec()) {
            QMessageBox::critical(this, "Database Error", "Query execution failed: " + query.lastError().text());
            return;
        }

        if (query.next()) {
            QString storedPassword = query.value("password").toString();

            if (oldPassword == storedPassword) {
                QSqlQuery updateQuery(dbManager.getDatabase());
                updateQuery.prepare("UPDATE users SET password = :newPassword WHERE id = :id");
                updateQuery.bindValue(":newPassword", newPassword);
                updateQuery.bindValue(":id", loggedUserId);

                if (!updateQuery.exec()) {
                    QMessageBox::critical(this, "Database Error", "Failed to update password: " + updateQuery.lastError().text());
                } else {
                    QMessageBox::information(this, "Success", "Password changed successfully.");
                }
            } else {
                QMessageBox::warning(this, "Invalid Password", "The old password is incorrect.");
            }
        } else {
            QMessageBox::warning(this, "User Not Found", "User not found in the database.");
        }
    }
}
