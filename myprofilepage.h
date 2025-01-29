#ifndef MYPROFILEPAGE_H
#define MYPROFILEPAGE_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class MyProfilePage;
}

class MyProfilePage : public QWidget
{
    Q_OBJECT

public:
    explicit MyProfilePage(const QString& loggedUserId, QWidget *parent = nullptr);
    ~MyProfilePage();

signals:
    void backToMenu();

private slots:
    void on_pushButton_return_clicked();
    void on_pushButton_changePassword_clicked();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MyProfilePage *ui;
    QString loggedUserId;
};

#endif // MYPROFILEPAGE_H
