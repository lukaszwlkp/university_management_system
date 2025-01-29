#ifndef MANAGECOURSESPAGE_H
#define MANAGECOURSESPAGE_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class ManageCoursesPage;
}

class ManageCoursesPage : public QWidget
{
    Q_OBJECT

public:
    explicit ManageCoursesPage(const QString& loggedUserId, QWidget *parent = nullptr);
    ~ManageCoursesPage();

signals:
    void backToMenu();

private slots:
    void on_pushButton_return_clicked();

    void on_pushButton_assignCourse_clicked();

    void on_pushButton_addCourse_clicked();

    void on_pushButton_deleteCourse_clicked();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::ManageCoursesPage *ui;
    QString loggedUserId;
};

#endif // MANAGECOURSESPAGE_H
