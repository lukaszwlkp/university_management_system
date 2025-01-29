#include "searchuserpage.h"
#include "ui_searchuserpage.h"
#include "databasemanager.h"
#include "searchuserdialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

extern DatabaseManager dbManager;

SearchUserPage::SearchUserPage(const QString& loggedUserId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SearchUserPage),
    loggedUserId(loggedUserId)
{
    ui->setupUi(this);
    ui->comboBox_searchType->addItems({"ID", "E-mail"});
}

SearchUserPage::~SearchUserPage()
{
    delete ui;
}

void SearchUserPage::on_pushButton_search_clicked()
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

        bool isLoggedUserStudent = loggedUserId.startsWith("1");
        bool isFoundUserStudent = foundUserId.startsWith("1");

        if (isLoggedUserStudent && isFoundUserStudent && loggedUserId != foundUserId) {
            QMessageBox::warning(this, "Permission Denied", "You cannot access other students' data.");
            return;
        }

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

        SearchUserDialog dialog(this);
        dialog.setUserData(foundUserId, firstName, secondName, email, additionalInfo);
        dialog.exec();

    } else {
        QMessageBox::warning(this, "Search Error", "No user found.");
    }
}

void SearchUserPage::on_pushButton_return_clicked()
{
    emit backToMenu();
}

void SearchUserPage::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        on_pushButton_return_clicked();
    }
    else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        on_pushButton_search_clicked();
    }
}
