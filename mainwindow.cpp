#include <QApplication>
#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    loginPage = new LoginPage(this);
    menuPage = new MenuPage(this);
    myProfilePage = nullptr;
    myGradesPage = nullptr;
    mySchedulePage = nullptr;
    searchUserPage = nullptr;
    manageGradesPage = nullptr;
    manageSchedulesPage = nullptr;
    manageCoursesPage = nullptr;
    manageUsersPage = nullptr;

    this->setWindowTitle("UT Management");

    // Start z loginPage
    showPage(loginPage);

    connect(loginPage, &LoginPage::loginSuccessful, this, &MainWindow::handleLogin);
    connect(loginPage, &LoginPage::exitApp, qApp, &QApplication::quit);
    connect(menuPage, &MenuPage::openMyProfile, this, &MainWindow::openMyProfile);
    connect(menuPage, &MenuPage::exitApp, qApp, &QApplication::quit);
    connect(menuPage, &MenuPage::openMyGrades, this, &MainWindow::openMyGrades);
    connect(menuPage, &MenuPage::openMySchedule, this, &MainWindow::openMySchedule);
    connect(menuPage, &MenuPage::openSearchUser, this, &MainWindow::openSearchUser);
    connect(menuPage, &MenuPage::openManageGrades, this, &MainWindow::openManageGrades);
    connect(menuPage, &MenuPage::openManageSchedules, this, &MainWindow::openManageSchedules);
    connect(menuPage, &MenuPage::openManageCourses, this, &MainWindow::openManageCourses);
    connect(menuPage, &MenuPage::openManageUsers, this, &MainWindow::openManageUsers);
}

void MainWindow::showPage(QWidget *page)
{
    loginPage->hide();
    menuPage->hide();
    if (myProfilePage) myProfilePage->hide();
    if (myGradesPage) myGradesPage->hide();
    if (mySchedulePage) mySchedulePage->hide();
    if (searchUserPage) searchUserPage->hide();
    if (manageGradesPage) manageGradesPage->hide();
    if (manageSchedulesPage) manageSchedulesPage->hide();
    if (manageCoursesPage) manageCoursesPage->hide();
    if (manageUsersPage) manageUsersPage->hide();

    page->show();
}

void MainWindow::handleLogin(const QString& userName)
{
    loggedUserId = userName;
    menuPage->configureMenuForUser(userName);
    showPage(menuPage);
}

void MainWindow::returnToMenu()
{
    showPage(menuPage);
}

void MainWindow::openMyProfile()
{
    if (!myProfilePage) {
        myProfilePage = new MyProfilePage(loggedUserId, this);
        connect(myProfilePage, &MyProfilePage::backToMenu, this, &MainWindow::returnToMenu);
    }
    showPage(myProfilePage);
}

void MainWindow::openMyGrades()
{
    if (!myGradesPage) {
        myGradesPage = new MyGradesPage(loggedUserId, this);
        connect(myGradesPage, &MyGradesPage::backToMenu, this, &MainWindow::returnToMenu);
    }
    showPage(myGradesPage);
}

void MainWindow::openMySchedule()
{
    if (!mySchedulePage) {
        mySchedulePage = new MySchedulePage(loggedUserId, this);
        connect(mySchedulePage, &MySchedulePage::backToMenu, this, &MainWindow::returnToMenu);
    }
    showPage(mySchedulePage);
}

void MainWindow::openSearchUser()
{
    if (!searchUserPage) {
        searchUserPage = new SearchUserPage(loggedUserId, this);
        connect(searchUserPage, &SearchUserPage::backToMenu, this, &MainWindow::returnToMenu);
    }
    showPage(searchUserPage);
}

void MainWindow::openManageGrades()
{
    if (!manageGradesPage) {
        manageGradesPage = new ManageGradesPage(loggedUserId, this);
        connect(manageGradesPage, &ManageGradesPage::backToMenu, this, &MainWindow::returnToMenu);
    }
    showPage(manageGradesPage);
}

void MainWindow::openManageSchedules()
{
    if (!manageSchedulesPage) {
        manageSchedulesPage = new ManageSchedulesPage(loggedUserId, this);
        connect(manageSchedulesPage, &ManageSchedulesPage::backToMenu, this, &MainWindow::returnToMenu);
    }
    showPage(manageSchedulesPage);
}

void MainWindow::openManageCourses()
{
    if (!manageCoursesPage) {
        manageCoursesPage = new ManageCoursesPage(loggedUserId, this);
        connect(manageCoursesPage, &ManageCoursesPage::backToMenu, this, &MainWindow::returnToMenu);
    }
    showPage(manageCoursesPage);
}

void MainWindow::openManageUsers()
{
    if (!manageUsersPage) {
        manageUsersPage = new ManageUsersPage(loggedUserId, this);
        connect(manageUsersPage, &ManageUsersPage::backToMenu, this, &MainWindow::returnToMenu);
    }
    showPage(manageUsersPage);
}
