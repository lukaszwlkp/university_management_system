#ifndef MENUPAGE_H
#define MENUPAGE_H

#include <QWidget>
#include <QMessageBox>
#include <QKeyEvent>

namespace Ui {
class MenuPage;
}

class MenuPage : public QWidget
{
    Q_OBJECT

public:
    explicit MenuPage(QWidget *parent = nullptr);
    ~MenuPage();

    void configureMenuForUser(const QString& userName);

signals:
    void openMyProfile();
    void exitApp();
    void openMyGrades();
    void openMySchedule();
    void openSearchUser();
    void openManageGrades();
    void openManageSchedules();
    void openManageCourses();
    void openManageUsers();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_pushButton_exit_clicked();
    void on_pushButton_myProfile_clicked();
    void on_pushButton_myGrades_clicked();
    void on_pushButton_mySchedule_clicked();
    void on_pushButton_searchUser_clicked();
    void on_pushButton_manageGrades_clicked();
    void on_pushButton_manageSchedules_clicked();
    void on_pushButton_manageCourses_clicked();
    void on_pushButton_manageUsers_clicked();

private:
    Ui::MenuPage *ui;
};

#endif // MENUPAGE_H
