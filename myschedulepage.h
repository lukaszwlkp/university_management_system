#ifndef MYSCHEDULEPAGE_H
#define MYSCHEDULEPAGE_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class MySchedulePage;
}

class MySchedulePage : public QWidget
{
    Q_OBJECT

public:
    explicit MySchedulePage(const QString& loggedUserId, QWidget *parent = nullptr);
    ~MySchedulePage();

signals:
    void backToMenu();

private slots:
    void on_pushButton_return_clicked();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::MySchedulePage *ui;
    QString loggedUserId;
};

#endif // MYSCHEDULEPAGE_H
