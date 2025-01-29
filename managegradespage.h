#ifndef MANAGEGRADESPAGE_H
#define MANAGEGRADESPAGE_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class ManageGradesPage;
}

class ManageGradesPage : public QWidget
{
    Q_OBJECT

public:
    explicit ManageGradesPage(const QString& loggedUserId, QWidget *parent = nullptr);
    ~ManageGradesPage();

signals:
    void backToMenu();
    void goToStatistics();

private slots:
    void on_pushButton_return_clicked();
    void on_pushButton_confirm_clicked();
    void on_pushButton_removeGrade_clicked();
    void on_pushButton_toStatistics_clicked();
protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::ManageGradesPage *ui;
    QString loggedUserId;
};

#endif

