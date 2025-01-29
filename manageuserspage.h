#ifndef MANAGEUSERSPAGE_H
#define MANAGEUSERSPAGE_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class ManageUsersPage;
}

class ManageUsersPage : public QWidget
{
    Q_OBJECT

public:
    explicit ManageUsersPage(const QString& loggedUserId, QWidget *parent = nullptr);
    ~ManageUsersPage();

signals:
    void backToMenu();

private slots:
    void on_pushButton_return_clicked();

    void on_pushButton_editUser_clicked();

    void on_pushButton_addUser_clicked();

    void on_pushButton_deleteUser_clicked();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::ManageUsersPage *ui;
    QString loggedUserId;
};

#endif // MANAGEUSERSPAGE_H
