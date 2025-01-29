#include "loginpage.h"
#include "ui_loginpage.h"
#include "databasemanager.h"

DatabaseManager dbManager;

LoginPage::LoginPage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginPage)
{
    ui->setupUi(this);
    ui->lineEdit_userName->setFocus();
    QWidget::setTabOrder(ui->lineEdit_userName, ui->lineEdit_password);
}

LoginPage::~LoginPage()
{
    delete ui;
}

void LoginPage::on_pushButton_login_clicked()
{
    QString userName = ui->lineEdit_userName->text().trimmed();
    QString password = ui->lineEdit_password->text().trimmed();

    if (userName.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Login", "Please enter both username and password.");
        return;
    }

    if (!dbManager.getDatabase().isOpen()) {
        if (!dbManager.connectDatabase()) {
            QMessageBox::critical(this, "Database Error", "Database connection failed.");
            return;
        }
    }

    QSqlQuery query(dbManager.getDatabase());
    query.prepare("SELECT * FROM users WHERE id = :id AND password = :password");
    query.bindValue(":id", userName);
    query.bindValue(":password", password);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Query failed: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        emit loginSuccessful(userName);
    } else {
        QMessageBox::warning(this, "Login", "Invalid username or password.");
    }
}

void LoginPage::on_pushButton_close_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Exit", "Close the app?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        emit exitApp();
    }
}

void LoginPage::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        on_pushButton_login_clicked();
    } else if (event->key() == Qt::Key_Escape) {
        on_pushButton_close_clicked();
    }
}
