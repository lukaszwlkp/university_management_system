#include "managecoursespage.h"
#include "ui_managecoursespage.h"
#include "databasemanager.h"
#include "managecoursesdialog.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

extern DatabaseManager dbManager;

ManageCoursesPage::ManageCoursesPage(const QString& loggedUserId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ManageCoursesPage),
    loggedUserId(loggedUserId)
{
    ui->setupUi(this);
    ui->comboBox_type->addItems({"lecture", "laboratory", "exercise"});
}

ManageCoursesPage::~ManageCoursesPage()
{
    delete ui;
}

void ManageCoursesPage::on_pushButton_assignCourse_clicked()
{
    QString name = ui->lineEdit_name->text().trimmed();
    QString fieldOfStudy = ui->lineEdit_fieldOfStudy->text().trimmed();
    int semester = ui->lineEdit_semester->text().toInt();
    QString type = ui->comboBox_type->currentText();

    QString semesterStr = ui->lineEdit_semester->text().trimmed();

    if (name.isEmpty() || fieldOfStudy.isEmpty() || semester == 0) {
        QMessageBox::warning(this, "Add Course Error", "Please fill in all the fields.");
        return;
    }

    if (!dbManager.getDatabase().isOpen() && !dbManager.connectDatabase()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to the database.");
        return;
    }

    QSqlQuery query(dbManager.getDatabase());

    query.prepare("SELECT id FROM subjects WHERE name = :name AND fieldOfStudy = :fieldOfStudy AND semester = :semester AND type = :type");
    query.bindValue(":name", name);
    query.bindValue(":fieldOfStudy", fieldOfStudy);
    query.bindValue(":semester", semester);
    query.bindValue(":type", type);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Query execution failed: " + query.lastError().text());
        return;
    }

    if (!query.next()) {
        QMessageBox::warning(this, "Assign Course Error", "This course does not exist (case sensitive).");
        return;
    }
    ManageCoursesDialog dialog(this);
    dialog.setCourseData(name, type, fieldOfStudy, semesterStr);
    dialog.exec();
}

void ManageCoursesPage::on_pushButton_addCourse_clicked()
{
    QString name = ui->lineEdit_name->text().trimmed();
    QString fieldOfStudy = ui->lineEdit_fieldOfStudy->text().trimmed();
    int semester = ui->lineEdit_semester->text().toInt();
    QString type = ui->comboBox_type->currentText();

    if (name.isEmpty() || fieldOfStudy.isEmpty() || semester == 0) {
        QMessageBox::warning(this, "Add Course Error", "Please fill in all the fields.");
        return;
    }

    if (!dbManager.getDatabase().isOpen() && !dbManager.connectDatabase()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to the database.");
        return;
    }

    QSqlQuery query(dbManager.getDatabase());

    query.prepare("SELECT id FROM subjects WHERE name = :name AND fieldOfStudy = :fieldOfStudy AND semester = :semester AND type = :type");
    query.bindValue(":name", name);
    query.bindValue(":fieldOfStudy", fieldOfStudy);
    query.bindValue(":semester", semester);
    query.bindValue(":type", type);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Query execution failed: " + query.lastError().text());
        return;
    }

    if (query.next()) {
        QMessageBox::warning(this, "Add Course Error", "This course already exists.");
        return;
    }

    query.prepare("INSERT INTO subjects (name, fieldOfStudy, semester, type) VALUES (:name, :fieldOfStudy, :semester, :type)");
    query.bindValue(":name", name);
    query.bindValue(":fieldOfStudy", fieldOfStudy);
    query.bindValue(":semester", semester);
    query.bindValue(":type", type);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to add the course: " + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Success", "Course added successfully.");
}


void ManageCoursesPage::on_pushButton_deleteCourse_clicked()
{
    QString name = ui->lineEdit_name->text().trimmed();
    QString fieldOfStudy = ui->lineEdit_fieldOfStudy->text().trimmed();
    int semester = ui->lineEdit_semester->text().toInt();
    QString type = ui->comboBox_type->currentText();

    if (name.isEmpty() || fieldOfStudy.isEmpty() || semester == 0) {
        QMessageBox::warning(this, "Delete Course Error", "Please fill in all the fields.");
        return;
    }

    if (!dbManager.getDatabase().isOpen() && !dbManager.connectDatabase()) {
        QMessageBox::critical(this, "Database Error", "Failed to connect to the database.");
        return;
    }

    QSqlQuery query(dbManager.getDatabase());

    query.prepare("DELETE FROM groupSubjects WHERE subjectId = (SELECT id FROM subjects WHERE name = :name AND fieldOfStudy = :fieldOfStudy AND semester = :semester AND type = :type)");
    query.bindValue(":name", name);
    query.bindValue(":fieldOfStudy", fieldOfStudy);
    query.bindValue(":semester", semester);
    query.bindValue(":type", type);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to delete from groupSubjects: " + query.lastError().text());
        return;
    }

    query.prepare("DELETE FROM subjects WHERE name = :name AND fieldOfStudy = :fieldOfStudy AND semester = :semester AND type = :type");
    query.bindValue(":name", name);
    query.bindValue(":fieldOfStudy", fieldOfStudy);
    query.bindValue(":semester", semester);
    query.bindValue(":type", type);

    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to delete from subjects: " + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Success", "Course deleted successfully.");
}

void ManageCoursesPage::on_pushButton_return_clicked()
{
    emit backToMenu();
}

void ManageCoursesPage::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        on_pushButton_return_clicked();
    }
}
