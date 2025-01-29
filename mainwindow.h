#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "databasemanager.h"
#include "loginpage.h"
#include "menupage.h"
#include "myprofilepage.h"
#include "mygradespage.h"
#include "myschedulepage.h"
#include "searchuserpage.h"
#include "managegradespage.h"
#include "manageschedulespage.h"
#include "managecoursespage.h"
#include "manageuserspage.h"

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    LoginPage *loginPage;
    MenuPage *menuPage;
    MyProfilePage *myProfilePage;
    MyGradesPage *myGradesPage;
    MySchedulePage *mySchedulePage;
    SearchUserPage *searchUserPage;
    ManageGradesPage *manageGradesPage;
    ManageSchedulesPage *manageSchedulesPage;
    ManageCoursesPage *manageCoursesPage;
    ManageUsersPage *manageUsersPage;

    void showPage(QWidget *page);
    QString loggedUserId;

private slots:
    void handleLogin(const QString& username);
    void openMyProfile();
    void returnToMenu();
    void openMyGrades();
    void openMySchedule();
    void openSearchUser();
    void openManageGrades();
    void openManageSchedules();
    void openManageCourses();
    void openManageUsers();
};

#endif // MAINWINDOW_H
