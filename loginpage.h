#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include <QtSql>

QT_BEGIN_NAMESPACE
namespace Ui {
class LoginPage;
}
QT_END_NAMESPACE

class LoginPage : public QWidget
{
    Q_OBJECT

public:
    explicit LoginPage(QWidget *parent = nullptr);
    ~LoginPage();

signals:
    void loginSuccessful(const QString& username);
    void exitApp();

private slots:
    void on_pushButton_login_clicked();
    void on_pushButton_close_clicked();

private:
    Ui::LoginPage *ui;
    QSqlDatabase DBConnection;

protected:
    void keyPressEvent(QKeyEvent *event) override;
};

#endif // LOGINPAGE_H
