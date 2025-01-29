#ifndef MANAGESCHEDULESPAGE_H
#define MANAGESCHEDULESPAGE_H

#include <QWidget>
#include <QKeyEvent>
#include <QStackedWidget>
#include "manageavailabilitypage.h"

namespace Ui {
class ManageSchedulesPage;
}

class ManageSchedulesPage : public QWidget
{
    Q_OBJECT

public:
    explicit ManageSchedulesPage(const QString& loggedUserId, QWidget *parent = nullptr);
    ~ManageSchedulesPage() override;


    QStackedWidget *stackedWidget;

private slots:
    void generateSchedule();
    void on_pushButton_manageAvailability_clicked();
    void on_pushButton_return_clicked();
signals:
    void backToMenu();
protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::ManageSchedulesPage *ui;
    QString loggedUserId;

    ManageAvailabilityPage *manageAvailabilityPage;
};

#endif // MANAGESCHEDULESPAGE_H
