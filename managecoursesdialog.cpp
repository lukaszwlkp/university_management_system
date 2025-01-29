#include "managecoursesdialog.h"
#include "ui_managecoursesdialog.h"
#include "databasemanager.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

extern DatabaseManager dbManager;

ManageCoursesDialog::ManageCoursesDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ManageCoursesDialog)
{
    ui->setupUi(this);
    setWindowTitle("Manage Courses");

    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &ManageCoursesDialog::onSave);
}

ManageCoursesDialog::~ManageCoursesDialog()
{
    delete ui;
}

void ManageCoursesDialog::setCourseData(const QString &name, const QString &type, const QString &fieldOfStudy,
                                        const QString &semester)
{
    ui->lineEdit_courseName->setText(name);
    ui->lineEdit_courseType->setText(type);
    ui->lineEdit_fieldOfStudy->setText(fieldOfStudy);
    ui->lineEdit_semester->setText(semester);
}

QString ManageCoursesDialog::getGroupNumber() const
{
    return ui->lineEdit_group->text().trimmed();
}

QString ManageCoursesDialog::getTeacherId() const
{
    return ui->lineEdit_workerId->text().trimmed();
}

void ManageCoursesDialog::onSave()
{
    QString groupNumber = ui->lineEdit_group->text().trimmed();
    QString teacherId = ui->lineEdit_workerId->text().trimmed();
    QString courseName = ui->lineEdit_courseName->text().trimmed();
    QString courseType = ui->lineEdit_courseType->text().trimmed();
    QString fieldOfStudy = ui->lineEdit_fieldOfStudy->text().trimmed();
    QString semester = ui->lineEdit_semester->text().trimmed();

    if (groupNumber.isEmpty() || teacherId.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please fill in both the group number and teacher ID.");
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT id FROM subjects WHERE name = :name AND fieldOfStudy = :fieldOfStudy AND semester = :semester AND type = :type");
    query.bindValue(":name", courseName);
    query.bindValue(":fieldOfStudy", fieldOfStudy);
    query.bindValue(":semester", semester);
    query.bindValue(":type", courseType);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Query execution failed: " + query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Course Not Found", "The course does not exist in the database.");
        return;
    }

    int subjectId = query.value("id").toInt();

    query.prepare("SELECT teacherId FROM groupSubjects WHERE subjectId = :subjectId AND groupNumber = :groupNumber");
    query.bindValue(":subjectId", subjectId);
    query.bindValue(":groupNumber", groupNumber);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to check the database: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        QString currentTeacherId = query.value("teacherId").toString();
        QMessageBox::StandardButton response = QMessageBox::question(
            this,
            "Group Assignment",
            QString("This group already has teacher ID: %1 assigned. Do you want to replace them?").arg(currentTeacherId),
            QMessageBox::Yes | QMessageBox::No
            );

        if (response == QMessageBox::Yes) {
            query.prepare("UPDATE groupSubjects SET teacherId = :teacherId WHERE subjectId = :subjectId AND groupNumber = :groupNumber");
            query.bindValue(":teacherId", teacherId);
            query.bindValue(":subjectId", subjectId);
            query.bindValue(":groupNumber", groupNumber);

            if (!query.exec()) {
                QMessageBox::critical(this, "Database Error", "Failed to update the teacher: " + query.lastError().text());
                return;
            }

            QMessageBox::information(this, "Success", "Teacher successfully replaced.");
            accept();
        }
    } else {
        query.prepare("INSERT INTO groupSubjects (subjectId, groupNumber, teacherId) VALUES (:subjectId, :groupNumber, :teacherId)");
        query.bindValue(":subjectId", subjectId);
        query.bindValue(":groupNumber", groupNumber);
        query.bindValue(":teacherId", teacherId);

        if (!query.exec()) {
            QMessageBox::critical(this, "Database Error", "Failed to assign the teacher: " + query.lastError().text());
            return;
        }

        QMessageBox::information(this, "Success", "Teacher successfully assigned to the group.");
        accept();
    }
}
