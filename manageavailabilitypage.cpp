#include "manageavailabilitypage.h"
#include "ui_manageavailabilitypage.h"
#include "manageschedulespage.h"
#include <QMessageBox>
#include <QComboBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHeaderView>
#include <QDebug>
#include <QSqlRecord>


ManageAvailabilityPage::ManageAvailabilityPage(QWidget *parent, const QString& loggedUserId) :
    QWidget(parent),
    ui(new Ui::ManageAvailabilityPage),
    loggedUserId(loggedUserId)
{
    ui->setupUi(this);


    QScrollArea *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);


    QWidget *mainWidget = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout(mainWidget);


    QHBoxLayout *searchLayout = new QHBoxLayout();
    searchLayout->addWidget(ui->lineEdit_worker_id);
    searchLayout->addWidget(ui->pushButton_search);
    searchLayout->addWidget(ui->pushButton_confirm);
    mainLayout->addLayout(searchLayout);


    mainLayout->addWidget(ui->tableWidget);

    mainLayout->addWidget(ui->pushButton_return);

    mainWidget->setLayout(mainLayout);

    scrollArea->setWidget(mainWidget);


    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(scrollArea);
    setLayout(layout);


    ui->tableWidget->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

    connect(ui->pushButton_search, &QPushButton::clicked, this, &ManageAvailabilityPage::onSearchClicked);
    connect(ui->pushButton_confirm, &QPushButton::clicked, this, &ManageAvailabilityPage::onConfirmClicked);
    connect(ui->pushButton_return, &QPushButton::clicked, this, &ManageAvailabilityPage::onReturnClicked);
    setupTable();
}




ManageAvailabilityPage::~ManageAvailabilityPage()
{
    delete ui;
}


void ManageAvailabilityPage::setupTable()
{
    ui->tableWidget->setRowCount(7);
    ui->tableWidget->setColumnCount(5);

    QStringList columnHeaders = {"Mon", "Tue", "Wed", "Thu", "Fri"};
    ui->tableWidget->setHorizontalHeaderLabels(columnHeaders);

    QStringList rowHeaders = {"1", "2", "3", "4", "5", "6", "7"};
    ui->tableWidget->setVerticalHeaderLabels(rowHeaders);

    for (int col = 0; col < ui->tableWidget->columnCount(); ++col)
    {
        ui->tableWidget->setColumnWidth(col, 100);
    }
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        ui->tableWidget->setRowHeight(row, 50);
    }


    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col)
        {
            QComboBox *comboBox = new QComboBox();
            comboBox->addItems({"Available", "Unavailable"});
            comboBox->setCurrentIndex(0);
            ui->tableWidget->setCellWidget(row, col, comboBox);
        }
    }
}


void ManageAvailabilityPage::onConfirmClicked()
{
    QString workerId = ui->lineEdit_worker_id->text();

    if (workerId.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please enter a Worker ID.");
        return;
    }

    QSqlQuery query;

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col)
        {
            QComboBox *comboBox = qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(row, col));
            if (comboBox)
            {
                QString availability = comboBox->currentText();
                QString day = getDayFromColumnIndex(col);
                QString timeBlock = QString::number(row + 1);


                QString columnName = day + timeBlock;


                QString queryString = QString("UPDATE worker_availability SET %1 = '%2' WHERE worker_id = '%3'")
                                          .arg(columnName)
                                          .arg(availability)
                                          .arg(workerId);

                if (!query.exec(queryString))
                {
                    QMessageBox::critical(this, "Database Error", query.lastError().text());
                    return;
                }
            }
        }
    }

    QMessageBox::information(this, "Success", "Availability updated successfully!");
}

bool ManageAvailabilityPage::addWorkerToAvailability(const QString &workerId)
{
    QSqlQuery query;
    QStringList days = {"Mon", "Tue", "Wed", "Thu", "Fri"};
    QStringList timeBlocks = {"1", "2", "3", "4", "5", "6", "7"};


    QString insertQuery = "INSERT INTO worker_availability (worker_id";
    for (const QString &day : days)
    {
        for (const QString &block : timeBlocks)
        {
            insertQuery += ", " + day + block;
        }
    }
    insertQuery += ") VALUES ('" + workerId + "'";

    for (int i = 0; i < days.size() * timeBlocks.size(); ++i)
    {
        insertQuery += ", 'Available'";
    }
    insertQuery += ");";

    if (!query.exec(insertQuery))
    {
        qDebug() << "Error adding worker availability:" << query.lastError().text();
        return false;
    }

    return true;
}


void ManageAvailabilityPage::onSearchClicked()
{
    QString workerId = ui->lineEdit_worker_id->text();

    if (workerId.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Please enter a Worker ID.");
        return;
    }

    if (!workerId.startsWith("0"))
    {
        QMessageBox::warning(this, "Error", "Worker ID must start with '0'.");
        return;
    }

    QSqlQuery query;


    query.prepare("SELECT COUNT(*) FROM users WHERE id = :workerId");
    query.bindValue(":workerId", workerId);

    if (!query.exec() || !query.next())
    {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    int workerExists = query.value(0).toInt();
    if (workerExists == 0)
    {
        QMessageBox::warning(this, "Worker Does Not Exist", "A worker with the provided ID does not exist.");
        return;
    }


    query.prepare("SELECT COUNT(*) FROM worker_availability WHERE worker_id = :workerId");
    query.bindValue(":workerId", workerId);

    if (!query.exec() || !query.next())
    {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    int availabilityDataExists = query.value(0).toInt();

    if (availabilityDataExists == 0)
    {
        int response = QMessageBox::question(
            this,
            "No Availability Data",
            "The worker exists but has no availability data. Would you like to create default availability?",
            QMessageBox::Yes | QMessageBox::No
            );

        if (response == QMessageBox::Yes)
        {
            if (addWorkerToAvailability(workerId))
            {
                QMessageBox::information(this, "Success", "Default availability has been created. You can now edit it.");
                populateTable(workerId);
            }
            else
            {
                QMessageBox::critical(this, "Error", "Failed to create default availability.");
            }
        }
        return;
    }

    populateTable(workerId);
}




void ManageAvailabilityPage::onReturnClicked()
{
    qDebug() << "Returning to ManageSchedulesPage...";

    QWidget *parentWidget = this->parentWidget();
    if (!parentWidget) {
        qDebug() << "No parent widget found!";
        return;
    }

    ManageSchedulesPage *manageSchedulesPage = qobject_cast<ManageSchedulesPage *>(parentWidget);
    if (!manageSchedulesPage) {
        qDebug() << "Parent is not ManageSchedulesPage!";
        return;
    }

    if (!manageSchedulesPage->stackedWidget) {
        qDebug() << "stackedWidget not found in ManageSchedulesPage!";
        return;
    }

    manageSchedulesPage->stackedWidget->setCurrentWidget(manageSchedulesPage);
    qDebug() << "Successfully returned to ManageSchedulesPage!";
}



int ManageAvailabilityPage::getRowIndex(const QString &timeBlock)
{
    bool ok;
    int rowIndex = timeBlock.toInt(&ok);
    return (ok && rowIndex >= 1 && rowIndex <= 7) ? (rowIndex - 1) : -1;
}


int ManageAvailabilityPage::getColumnIndex(const QString &day)
{
    QStringList days = {"Mon", "Tue", "Wed", "Thu", "Fri"};
    return days.indexOf(day);
}

QString ManageAvailabilityPage::getTimeBlockFromRowIndex(int row)
{
    if (row >= 0 && row < 7)
    {
        return QString::number(row + 1);
    }
    return QString();
}


QString ManageAvailabilityPage::getDayFromColumnIndex(int col)
{
    QStringList days = {"Mon", "Tue", "Wed", "Thu", "Fri"};
    return days.value(col);
}

QString ManageAvailabilityPage::getTimeBlockFromIndices(int row, int col)
{

    QStringList days = {"Mon", "Tue", "Wed", "Thu", "Fri"};


    if (row < 0 || row >= 7 || col < 0 || col >= days.size())
    {
        return QString();
    }

    return days[col] + QString::number(row + 1);
}


void ManageAvailabilityPage::populateTable(const QString &workerId)
{
    QSqlQuery query;


    query.prepare("SELECT * FROM worker_availability WHERE worker_id = :workerId");
    query.bindValue(":workerId", workerId);

    if (!query.exec())
    {
        QMessageBox::critical(this, "Database Error", query.lastError().text());
        return;
    }

    if (!query.next())
    {
        QMessageBox::warning(this, "No Data", "No availability data found for this worker.");
        return;
    }


    for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
    {
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col)
        {
            QComboBox *comboBox = qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(row, col));
            if (comboBox)
            {
                comboBox->setCurrentText("Available");
            }
        }
    }

    for (int col = 0; col < ui->tableWidget->columnCount(); ++col)
    {
        QString day = getDayFromColumnIndex(col);
        for (int row = 0; row < ui->tableWidget->rowCount(); ++row)
        {
            QString timeBlock = QString::number(row + 1);
            QString columnName = day + timeBlock;

            int columnIndex = query.record().indexOf(columnName);
            if (columnIndex != -1)
            {
                QString availability = query.value(columnIndex).toString();
                QComboBox *comboBox = qobject_cast<QComboBox *>(ui->tableWidget->cellWidget(row, col));
                if (comboBox)
                {
                    comboBox->setCurrentText(availability);
                }
            }
        }
    }
}

