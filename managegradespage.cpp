#include "managegradespage.h"
#include "ui_managegradespage.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

extern DatabaseManager dbManager;

ManageGradesPage::ManageGradesPage(const QString& loggedUserId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManageGradesPage),
    loggedUserId(loggedUserId)
{
    ui->setupUi(this);


    ui->comboBox_subjectType->addItems({"Lecture", "Exercise", "Laboratory"});

    if (!dbManager.getDatabase().isOpen() && !dbManager.connectDatabase()) {
        qDebug() << "Error: Unable to connect to the database.";
    }
}

ManageGradesPage::~ManageGradesPage()
{
    delete ui;
}

void ManageGradesPage::on_pushButton_return_clicked()
{
    emit backToMenu();
}

void ManageGradesPage::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        on_pushButton_return_clicked();
    }
}


void ManageGradesPage::on_pushButton_confirm_clicked()
{

    QString studentId = ui->lineEdit_studentId->text().trimmed();
    double grade = ui->comboBox_grade->currentText().toDouble();
    QString subjectName = ui->lineEdit_subjectName->text().trimmed();
    QString subjectType = ui->comboBox_subjectType->currentText().toLower().trimmed();

    if (studentId.isEmpty() || grade == 0 || subjectName.isEmpty() || subjectType.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all the required fields.");
        return;
    }

    if (!dbManager.getDatabase().isOpen() && !dbManager.connectDatabase()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to the database.");
        return;
    }

    QSqlQuery query(dbManager.getDatabase());


    query.prepare("SELECT id FROM users WHERE id = :studentId");
    query.bindValue(":studentId", studentId);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to check student existence: " + query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Error", "Student with the provided ID does not exist.");
        return;
    }


    query.prepare("SELECT gsj.groupNumber, gsj.subjectId "
                  "FROM groupSubjects gsj "
                  "JOIN subjects s ON gsj.subjectId = s.id "
                  "WHERE gsj.teacherId = :teacherId "
                  "AND s.name = :subjectName "
                  "AND s.type = :subjectType");
    query.bindValue(":teacherId", loggedUserId);
    query.bindValue(":subjectName", subjectName);
    query.bindValue(":subjectType", subjectType);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to execute query: " + query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Error", "No matching group found for the subject and teacher.");
        return;
    }

    QString groupNumber = query.value("groupNumber").toString();
    int subjectId = query.value("subjectId").toInt();


    query.prepare("SELECT grade FROM grades WHERE student_id = :studentId AND subject_id = :subjectId AND subject_type = :subjectType");
    query.bindValue(":studentId", studentId);
    query.bindValue(":subjectId", subjectId);
    query.bindValue(":subjectType", subjectType);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to check if grade exists: " + query.lastError().text());
        return;
    }

    if (query.next()) {

        double existingGrade = query.value("grade").toDouble();


        QMessageBox::StandardButton response = QMessageBox::question(
            this,
            "Grade Exists",
            QString("A grade already exists for this student:\n\n"
                    "Student ID: %1\n"
                    "Subject: %2\n"
                    "Subject Type: %3\n"
                    "Existing Grade: %4\n\n"
                    "Do you want to update it?")
                .arg(studentId)
                .arg(subjectName)
                .arg(subjectType)
                .arg(existingGrade),
            QMessageBox::Yes | QMessageBox::No
            );

        if (response == QMessageBox::No) {
            QMessageBox::information(this, "No Action Taken", "The grade was not updated.");
            return;
        }


        query.prepare("UPDATE grades SET grade = :newGrade WHERE student_id = :studentId AND subject_id = :subjectId AND subject_type = :subjectType");
        query.bindValue(":newGrade", grade);
        query.bindValue(":studentId", studentId);
        query.bindValue(":subjectId", subjectId);
        query.bindValue(":subjectType", subjectType);

        if (!query.exec()) {
            QMessageBox::critical(this, "Database Error", "Failed to update the grade: " + query.lastError().text());
            return;
        }

        QMessageBox::information(this, "Success", "The grade was successfully updated.");
        return;
    }


    query.prepare("INSERT INTO GRADES (student_id, grade, group_id, subject_id, subject_type, teacher_id) "
                  "VALUES (:studentId, :grade, :groupNumber, :subjectId, :subjectType, :teacherId)");
    query.bindValue(":studentId", studentId);
    query.bindValue(":grade", grade);
    query.bindValue(":groupNumber", groupNumber);
    query.bindValue(":subjectId", subjectId);
    query.bindValue(":subjectType", subjectType);
    query.bindValue(":teacherId", loggedUserId);

    dbManager.getDatabase().transaction();

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to insert grade: " + query.lastError().text());
        dbManager.getDatabase().rollback();
        return;
    }

    if (!dbManager.getDatabase().commit()) {
        QMessageBox::critical(this, "Database Error", "Failed to commit transaction.");
        dbManager.getDatabase().rollback();
        return;
    }

    QMessageBox::information(this, "Success", "Grade successfully added.");

}
void ManageGradesPage::on_pushButton_removeGrade_clicked()
{

    QString studentId = ui->lineEdit_studentId->text().trimmed();
    QString subjectName = ui->lineEdit_subjectName->text().trimmed();
    QString subjectType = ui->comboBox_subjectType->currentText().toLower().trimmed();

    if (studentId.isEmpty() || subjectName.isEmpty() || subjectType.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in all the required fields.");
        return;
    }

    if (!dbManager.getDatabase().isOpen() && !dbManager.connectDatabase()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to the database.");
        return;
    }

    QSqlQuery query(dbManager.getDatabase());


    query.prepare("SELECT gsj.groupNumber, gsj.subjectId "
                  "FROM groupSubjects gsj "
                  "JOIN subjects s ON gsj.subjectId = s.id "
                  "WHERE gsj.teacherId = :teacherId "
                  "AND s.name = :subjectName "
                  "AND s.type = :subjectType");
    query.bindValue(":teacherId", loggedUserId);
    query.bindValue(":subjectName", subjectName);
    query.bindValue(":subjectType", subjectType);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to execute query: " + query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Error", "No matching group found for the subject and teacher.");
        return;
    }

    int subjectId = query.value("subjectId").toInt();


    query.prepare("SELECT grade FROM grades WHERE student_id = :studentId AND subject_id = :subjectId AND subject_type = :subjectType");
    query.bindValue(":studentId", studentId);
    query.bindValue(":subjectId", subjectId);
    query.bindValue(":subjectType", subjectType);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to check if grade exists: " + query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Error", "No grade found for the specified student and subject.");
        return;
    }


    double existingGrade = query.value("grade").toDouble();


    QMessageBox::StandardButton response = QMessageBox::question(
        this,
        "Confirm Deletion",
        QString("The following grade will be removed:\n\n"
                "Student ID: %1\n"
                "Subject: %2\n"
                "Subject Type: %3\n"
                "Grade: %4\n\n"
                "Are you sure you want to delete this grade?")
            .arg(studentId)
            .arg(subjectName)
            .arg(subjectType)
            .arg(existingGrade),
        QMessageBox::Yes | QMessageBox::No
        );

    if (response == QMessageBox::No) {
        QMessageBox::information(this, "No Action Taken", "The grade was not deleted.");
        return;
    }


    query.prepare("DELETE FROM grades WHERE student_id = :studentId AND subject_id = :subjectId AND subject_type = :subjectType");
    query.bindValue(":studentId", studentId);
    query.bindValue(":subjectId", subjectId);
    query.bindValue(":subjectType", subjectType);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to delete grade: " + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Success", "The grade was successfully deleted.");
}


