#ifndef MANAGEAVAILABILITYPAGE_H
#define MANAGEAVAILABILITYPAGE_H

#include <QWidget>
#include <QComboBox>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class ManageAvailabilityPage;
}

class ManageAvailabilityPage : public QWidget
{
    Q_OBJECT

public:
    explicit ManageAvailabilityPage(QWidget *parent = nullptr, const QString &userId = QString());
    ~ManageAvailabilityPage();

signals:
    void backToManageSchedules();
private slots:
    void onSearchClicked();
    void onConfirmClicked();
    void onReturnClicked();

private:
    Ui::ManageAvailabilityPage *ui;


    QString getTimeBlockFromIndices(int row, int col);


    int getRowIndex(const QString &timeBlock);
    int getColumnIndex(const QString &timeBlock);
    void setupTable();
    bool addWorkerToAvailability(const QString &workerId);
    QString getTimeBlockFromRowIndex(int row);
    QString getDayFromColumnIndex(int col);
    void populateTable(const QString &workerId);


    QString loggedUserId;
};


#endif
