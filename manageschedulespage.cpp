#include "manageschedulespage.h"
#include "ui_manageschedulespage.h"
#include "manageavailabilitypage.h"
#include <QKeyEvent>
#include <QDebug>
#include <QVBoxLayout>
#include<QMap>
#include<QSqlRecord>
#include "databasemanager.h"
extern DatabaseManager dbManager;


void ManageSchedulesPage::generateSchedule() {
    DatabaseManager dbManager;
    if (!dbManager.connectDatabase()) {
        qDebug() << "Failed to connect to the database.";
        return;
    }

    QSqlDatabase db = dbManager.getDatabase();
    QSqlQuery query(db);


    QStringList timeSlots = {"Mon1", "Mon2", "Mon3", "Mon4", "Mon5", "Mon6", "Mon7",
                             "Tue1", "Tue2", "Tue3", "Tue4", "Tue5", "Tue6", "Tue7",
                             "Wed1", "Wed2", "Wed3", "Wed4", "Wed5", "Wed6", "Wed7",
                             "Thu1", "Thu2", "Thu3", "Thu4", "Thu5", "Thu6", "Thu7",
                             "Fri1", "Fri2", "Fri3", "Fri4", "Fri5", "Fri6", "Fri7"};


    QString insertWorkerQuery = "INSERT INTO worker_timetable (worker_id, "
                                "Mon1, Mon2, Mon3, Mon4, Mon5, Mon6, Mon7, "
                                "Tue1, Tue2, Tue3, Tue4, Tue5, Tue6, Tue7, "
                                "Wed1, Wed2, Wed3, Wed4, Wed5, Wed6, Wed7, "
                                "Thu1, Thu2, Thu3, Thu4, Thu5, Thu6, Thu7, "
                                "Fri1, Fri2, Fri3, Fri4, Fri5, Fri6, Fri7) "
                                "VALUES (:workerId, '-', '-', '-', '-', '-', '-', '-', "
                                "'-', '-', '-', '-', '-', '-', '-', "
                                "'-', '-', '-', '-', '-', '-', '-', "
                                "'-', '-', '-', '-', '-', '-', '-', "
                                "'-', '-', '-', '-', '-', '-', '-')";


    QString insertGroupQuery = "INSERT INTO group_timetable (groupNumber, "
                               "Mon1, Mon2, Mon3, Mon4, Mon5, Mon6, Mon7, "
                               "Tue1, Tue2, Tue3, Tue4, Tue5, Tue6, Tue7, "
                               "Wed1, Wed2, Wed3, Wed4, Wed5, Wed6, Wed7, "
                               "Thu1, Thu2, Thu3, Thu4, Thu5, Thu6, Thu7, "
                               "Fri1, Fri2, Fri3, Fri4, Fri5, Fri6, Fri7) "
                               "VALUES (:groupNumber, '-', '-', '-', '-', '-', '-', '-', "
                               "'-', '-', '-', '-', '-', '-', '-', "
                               "'-', '-', '-', '-', '-', '-', '-', "
                               "'-', '-', '-', '-', '-', '-', '-', "
                               "'-', '-', '-', '-', '-', '-', '-')";


    if (!query.exec("SELECT * FROM worker_availability")) {
        qDebug() << "Failed to load worker availability:" << query.lastError().text();
        return;
    }

    QMap<QString, QMap<QString, bool>> workerAvailability;
    while (query.next()) {
        QString workerId = query.value(0).toString();
        QMap<QString, bool> availability;

        for (int i = 1; i < query.record().count(); ++i) {
            QString timeSlot = query.record().fieldName(i);
            QString isAvailable = query.value(i).toString();
            availability[timeSlot] = (isAvailable == "Available");
        }

        workerAvailability[workerId] = availability;
    }


    if (!query.exec("SELECT subjectId, groupNumber, teacherId FROM groupSubjects")) {
        qDebug() << "Failed to load subjects and groups:" << query.lastError().text();
        return;
    }

    QVector<QMap<QString, QString>> groupSubjects;
    while (query.next()) {
        QMap<QString, QString> groupData;
        groupData["subjectId"] = query.value(0).toString();
        groupData["groupNumber"] = query.value(1).toString();
        groupData["teacherId"] = query.value(2).toString();
        groupSubjects.append(groupData);
    }

    for (const auto& group : groupSubjects) {
        QString teacherId = group["teacherId"];
        QString groupNumber = group["groupNumber"];
        QString subjectId = group["subjectId"];

        if (!workerAvailability.contains(teacherId)) {
            qDebug() << "No availability found for teacher:" << teacherId;
            continue;
        }

        QMap<QString, bool>& availability = workerAvailability[teacherId];

        QString assignedSlot;
        for (auto it = availability.begin(); it != availability.end(); ++it) {
            if (it.value()) {
                assignedSlot = it.key();
                availability[it.key()] = false;
                break;
            }
        }

        if (assignedSlot.isEmpty()) {
            qDebug() << "No available timeslots for teacher:" << teacherId;
            continue;
        }


        QString checkWorkerQuery = "SELECT 1 FROM worker_timetable WHERE worker_id = :workerId";
        query.prepare(checkWorkerQuery);
        query.bindValue(":workerId", teacherId);
        if (!query.exec() || !query.next()) {
            query.prepare(insertWorkerQuery);
            query.bindValue(":workerId", teacherId);
            if (!query.exec()) {
                qDebug() << "Failed to insert into worker_timetable:" << query.lastError().text();
                continue;
            }
        }


        QString workerQuery = QString("UPDATE worker_timetable SET %1 = :groupNumber WHERE worker_id = :workerId;").arg(assignedSlot);
        query.prepare(workerQuery);
        query.bindValue(":groupNumber", groupNumber);
        query.bindValue(":workerId", teacherId);

        if (!query.exec()) {
            qDebug() << "Failed to update worker_timetable:" << query.lastError().text();
        } else {
            qDebug() << "Assigned group" << groupNumber << "to teacher" << teacherId << "in timeslot" << assignedSlot;
        }


        QString checkGroupQuery = "SELECT 1 FROM group_timetable WHERE groupNumber = :groupNumber";
        query.prepare(checkGroupQuery);
        query.bindValue(":groupNumber", groupNumber);
        if (!query.exec() || !query.next()) {
            query.prepare(insertGroupQuery);
            query.bindValue(":groupNumber", groupNumber);
            if (!query.exec()) {
                qDebug() << "Failed to insert into group_timetable:" << query.lastError().text();
                continue;
            }
        }


        QString groupQuery = QString("UPDATE group_timetable SET %1 = :teacherId WHERE groupNumber = :groupNumber;").arg(assignedSlot);
        query.prepare(groupQuery);
        query.bindValue(":teacherId", teacherId);
        query.bindValue(":groupNumber", groupNumber);

        if (!query.exec()) {
            qDebug() << "Failed to update group_timetable:" << query.lastError().text();
        } else {
            qDebug() << "Updated group_timetable for group" << groupNumber << "and teacher" << teacherId;
        }
    }


    for (const QString& slot : timeSlots) {
        QString fillQuery = QString("UPDATE worker_timetable SET %1 = '-' WHERE %1 IS NULL;").arg(slot);
        if (!query.exec(fillQuery)) {
            qDebug() << "Failed to fill empty worker_timetable slots:" << query.lastError().text();
        }

        fillQuery = QString("UPDATE group_timetable SET %1 = '-' WHERE %1 IS NULL;").arg(slot);
        if (!query.exec(fillQuery)) {
            qDebug() << "Failed to fill empty group_timetable slots:" << query.lastError().text();
        }
    }

    qDebug() << "Schedule generation completed.";
    db.close();
}



ManageSchedulesPage::ManageSchedulesPage(const QString& loggedUserId, QWidget *parent) :
    QWidget(parent),
    stackedWidget(new QStackedWidget(this)),
    ui(new Ui::ManageSchedulesPage),
    loggedUserId(loggedUserId),
    manageAvailabilityPage(nullptr)
{
    ui->setupUi(this);


    manageAvailabilityPage = new ManageAvailabilityPage(this, loggedUserId);

    stackedWidget->addWidget(ui->centralwidget);

    stackedWidget->addWidget(manageAvailabilityPage);

    setLayout(new QVBoxLayout);
    layout()->addWidget(stackedWidget);

    connect(ui->pushButton_manageAvailability, &QPushButton::clicked, this, &ManageSchedulesPage::on_pushButton_manageAvailability_clicked);
    connect(ui->pushButton_return, &QPushButton::clicked, this, &ManageSchedulesPage::on_pushButton_return_clicked);
    connect(ui->pushButton_generate, &QPushButton::clicked, this, &ManageSchedulesPage::generateSchedule);
}

void ManageSchedulesPage::on_pushButton_manageAvailability_clicked()
{
    qDebug() << "Navigating to ManageAvailabilityPage...";

    stackedWidget->setCurrentWidget(manageAvailabilityPage);
}

void ManageSchedulesPage::on_pushButton_return_clicked()
{
    emit backToMenu();
}

void ManageSchedulesPage::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        on_pushButton_return_clicked();
    }
}
ManageSchedulesPage::~ManageSchedulesPage()
{
    delete ui;
}

