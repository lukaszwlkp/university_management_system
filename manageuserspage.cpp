#include "manageuserspage.h"
#include "ui_manageuserspage.h"
#include "databasemanager.h"
#include "manageusersdialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

extern DatabaseManager dbManager;

ManageUsersPage::ManageUsersPage(const QString& loggedUserId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManageUsersPage),
    loggedUserId(loggedUserId)
{
    ui->setupUi(this);
    ui->comboBox_searchType->addItems({"ID", "E-mail"});
}

ManageUsersPage::~ManageUsersPage()
{
    delete ui;
}

void ManageUsersPage::on_pushButton_editUser_clicked()
{
    QString searchType = ui->comboBox_searchType->currentText();
    QString searchTarget = ui->lineEdit_searchTarget->text().trimmed();

    if (searchTarget.isEmpty()) {
        QMessageBox::warning(this, "Search Error", "Please enter a search target.");
        return;
    }

    if (!dbManager.getDatabase().isOpen() && !dbManager.connectDatabase()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to the database.");
        return;
    }

    QSqlQuery query(dbManager.getDatabase());
    QString queryString;

    if (searchType == "ID") {
        queryString = "SELECT * FROM users WHERE id = :target";
    } else if (searchType == "E-mail") {
        queryString = "SELECT * FROM users WHERE email = :target";
    } else {
        QMessageBox::warning(this, "Search Error", "Invalid search type.");
        return;
    }

    query.prepare(queryString);
    query.bindValue(":target", searchTarget);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Query execution failed: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        QString foundUserId = query.value("id").toString();

        bool isFoundUserStudent = foundUserId.startsWith("1");

        QString firstName = query.value("firstName").toString();
        QString secondName = query.value("secondName").toString();
        QString email = query.value("email").toString();
        QString additionalInfo;

        if (isFoundUserStudent) {
            additionalInfo = query.value("fieldOfStudy").toString() + ", sem. " +
                             query.value("semester").toString() + ", gr. " +
                             query.value("groupNumber").toString();
        } else {
            additionalInfo = query.value("degree").toString();
        }

        ManageUsersDialog dialog(this);
        dialog.setUserData(foundUserId, firstName, secondName, email, additionalInfo);
        dialog.exec();

    } else {
        QMessageBox::warning(this, "Search Error", "No user found.");
    }
}


void ManageUsersPage::on_pushButton_addUser_clicked()
{
    QString searchType = ui->comboBox_searchType->currentText();
    QString searchTarget = ui->lineEdit_searchTarget->text().trimmed();

    if (searchType != "ID") {
        QMessageBox::warning(this, "Add User Error", "Please select 'ID' as the search type to add a user.");
        return;
    }

    if (searchTarget.isEmpty()) {
        QMessageBox::warning(this, "Add User Error", "Please enter a valid ID.");
        return;
    }

    if (!dbManager.getDatabase().isOpen() && !dbManager.connectDatabase()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to the database.");
        return;
    }

    QSqlQuery query(dbManager.getDatabase());

    query.prepare("SELECT id FROM users WHERE id = :id");
    query.bindValue(":id", searchTarget);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Query execution failed: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        QMessageBox::warning(this, "Add User Error", "A user with this ID already exists.");
        return;
    }

    QString tempEmail = QString("temp.%1@example.com").arg(searchTarget);
    QString tempPassword = QString("temp%1").arg(searchTarget);

    query.prepare("INSERT INTO users (id, password,firstName, secondName, email, degree) "
                  "VALUES (:id, :password, '', '', :email, '')");
    query.bindValue(":id", searchTarget);
    query.bindValue(":email", tempEmail);
    query.bindValue(":password", tempPassword);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to add new user: " + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Success", "User added successfully. You can now edit their details.");

    ui->comboBox_searchType->setCurrentText("ID");
    ui->lineEdit_searchTarget->setText(searchTarget);

    on_pushButton_editUser_clicked();
}

void ManageUsersPage::on_pushButton_deleteUser_clicked()
{
    QString searchType = ui->comboBox_searchType->currentText();
    QString searchTarget = ui->lineEdit_searchTarget->text().trimmed();

    if (searchTarget.isEmpty()) {
        QMessageBox::warning(this, "Delete User Error", "Please enter a valid ID or email.");
        return;
    }

    if (!dbManager.getDatabase().isOpen() && !dbManager.connectDatabase()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to the database.");
        return;
    }

    QSqlQuery query(dbManager.getDatabase());
    QString queryString;

    if (searchType == "ID") {
        queryString = "DELETE FROM users WHERE id = :target";
    } else if (searchType == "E-mail") {
        queryString = "DELETE FROM users WHERE email = :target";
    } else {
        QMessageBox::warning(this, "Delete User Error", "Invalid search type. Please select 'ID' or 'E-mail'.");
        return;
    }

    query.prepare(queryString);
    query.bindValue(":target", searchTarget);

    QMessageBox::StandardButton confirmation = QMessageBox::question(
        this,
        "Confirm Deletion",
        QString("Are you sure you want to delete the user with %1: %2?")
            .arg(searchType.toLower())
            .arg(searchTarget),
        QMessageBox::Yes | QMessageBox::No);

    if (confirmation != QMessageBox::Yes) {
        return;
    }

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to delete the user: " + query.lastError().text());
        return;
    }

    if (query.numRowsAffected() == 0) {
        QMessageBox::warning(this, "Delete User Error", "No user found with the specified " + searchType + ".");
    } else {
        QMessageBox::information(this, "Success", "User deleted successfully.");
    }
}


void ManageUsersPage::on_pushButton_return_clicked()
{
    emit backToMenu();
}

void ManageUsersPage::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        on_pushButton_return_clicked();
    }
}
