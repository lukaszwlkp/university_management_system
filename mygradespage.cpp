#include "mygradespage.h"
#include "ui_mygradespage.h"
#include "databasemanager.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QKeyEvent>

extern DatabaseManager dbManager;



MyGradesPage::MyGradesPage(const QString& loggedUserId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MyGradesPage),
    loggedUserId(loggedUserId)
{
    ui->setupUi(this);
    setupTable();
    loadGrades();
}

MyGradesPage::~MyGradesPage()
{
    delete ui;
}

void MyGradesPage::on_pushButton_return_clicked()
{
    emit backToMenu();
}

void MyGradesPage::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        on_pushButton_return_clicked();
    }
}

void MyGradesPage::setupTable()
{
    ui->tableWidget_grades->setColumnCount(3);


    ui->tableWidget_grades->horizontalHeader()->setVisible(false);

    ui->tableWidget_grades->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget_grades->setSelectionMode(QAbstractItemView::NoSelection);
    ui->tableWidget_grades->setSelectionBehavior(QAbstractItemView::SelectRows);


    ui->tableWidget_grades->verticalHeader()->setVisible(false);


    ui->tableWidget_grades->setColumnWidth(0, 235);
    ui->tableWidget_grades->setColumnWidth(1, 125);
    ui->tableWidget_grades->setColumnWidth(2, 60);
}


void MyGradesPage::loadGrades()
{
    QMap<int, QMap<QString, QMap<QString, QString>>> gradesData = fetchGrades();
    populateGradesTable(gradesData);
}

QMap<int, QMap<QString, QMap<QString, QString>>> MyGradesPage::fetchGrades()
{
    QMap<int, QMap<QString, QMap<QString, QString>>> gradesData;

    if (!dbManager.getDatabase().isOpen() && !dbManager.connectDatabase()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to the database.");
        return gradesData;
    }

    QSqlQuery query(dbManager.getDatabase());

    query.prepare(
        "SELECT s.semester AS semester, s.name AS subject_name, s.type AS subject_type, g.grade AS grade "
        "FROM subjects s "
        "LEFT JOIN grades g ON s.id = g.subject_id AND g.student_id = :studentId "
        "JOIN groupSubjects gs ON gs.subjectId = s.id "
        "WHERE gs.groupNumber = (SELECT groupNumber FROM users WHERE id = :studentId) "
        "ORDER BY s.semester, s.name, s.type"
        );

    query.bindValue(":studentId", loggedUserId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to fetch grades: " + query.lastError().text());
        return gradesData;
    }

    while (query.next()) {
        int semester = query.value("semester").toInt();
        QString subjectName = query.value("subject_name").toString();
        QString subjectType = query.value("subject_type").toString();
        QString grade = query.value("grade").toString();

        if (grade.isEmpty()) {
            grade = "-";
        }

        gradesData[semester][subjectName][subjectType] = grade;
    }

    return gradesData;
}

void MyGradesPage::populateGradesTable(const QMap<int, QMap<QString, QMap<QString, QString>>>& gradesData)
{
    ui->tableWidget_grades->setRowCount(0);

    for (auto semesterIt = gradesData.begin(); semesterIt != gradesData.end(); ++semesterIt) {
        int semester = semesterIt.key();
        QMap<QString, QMap<QString, QString>> subjects = semesterIt.value();


        int currentRow = ui->tableWidget_grades->rowCount();
        ui->tableWidget_grades->insertRow(currentRow);

        QTableWidgetItem *semesterHeader = new QTableWidgetItem(QString("Semester %1").arg(semester));
        semesterHeader->setBackground(Qt::gray);
        semesterHeader->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget_grades->setItem(currentRow, 0, semesterHeader);
        ui->tableWidget_grades->setSpan(currentRow, 0, 1, 3);


        for (auto subjectIt = subjects.begin(); subjectIt != subjects.end(); ++subjectIt) {
            QString subjectName = subjectIt.key();
            QMap<QString, QString> types = subjectIt.value();

            for (auto typeIt = types.begin(); typeIt != types.end(); ++typeIt) {
                currentRow = ui->tableWidget_grades->rowCount();
                ui->tableWidget_grades->insertRow(currentRow);

                ui->tableWidget_grades->setItem(currentRow, 0, new QTableWidgetItem(subjectName));
                ui->tableWidget_grades->setItem(currentRow, 1, new QTableWidgetItem(typeIt.key()));
                ui->tableWidget_grades->setItem(currentRow, 2, new QTableWidgetItem(typeIt.value()));
            }
        }
    }
}

