#include "menupage.h"
#include "ui_menupage.h"

MenuPage::MenuPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MenuPage)
{
    ui->setupUi(this);
}

MenuPage::~MenuPage()
{
    delete ui;
}

void MenuPage::configureMenuForUser(const QString& userName)
{
    if (userName == "000000") { // admin
        ui->pushButton_myGrades->setEnabled(false);
        ui->pushButton_mySchedule->setEnabled(false);
    }
    else if (userName.startsWith("1")) { // student
        ui->pushButton_manageCourses->setEnabled(false);
        ui->pushButton_manageGrades->setEnabled(false);
        ui->pushButton_manageSchedules->setEnabled(false);
        ui->pushButton_manageUsers->setEnabled(false);
    }
    else if (userName.startsWith("0")) { // worker
        ui->pushButton_manageCourses->setEnabled(false);
        ui->pushButton_manageSchedules->setEnabled(false);
        ui->pushButton_manageUsers->setEnabled(false);
        ui->pushButton_myGrades->setEnabled(false);
    }
}

void MenuPage::on_pushButton_exit_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "Exit", "Close the app?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        emit exitApp();
    }
}

void MenuPage::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        on_pushButton_exit_clicked();
    }
}

void MenuPage::on_pushButton_myProfile_clicked()
{
    emit openMyProfile();
}

void MenuPage::on_pushButton_myGrades_clicked()
{
    emit openMyGrades();
}

void MenuPage::on_pushButton_mySchedule_clicked()
{
    emit openMySchedule();
}

void MenuPage::on_pushButton_searchUser_clicked()
{
    emit openSearchUser();
}

void MenuPage::on_pushButton_manageGrades_clicked()
{
    emit openManageGrades();
}

void MenuPage::on_pushButton_manageSchedules_clicked()
{
    emit openManageSchedules();
}

void MenuPage::on_pushButton_manageCourses_clicked()
{
    emit openManageCourses();
}

void MenuPage::on_pushButton_manageUsers_clicked()
{
    emit openManageUsers();
}
