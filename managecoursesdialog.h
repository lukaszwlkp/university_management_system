#ifndef MANAGECOURSESDIALOG_H
#define MANAGECOURSESDIALOG_H

#include <QDialog>

namespace Ui {
class ManageCoursesDialog;
}

class ManageCoursesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManageCoursesDialog(QWidget *parent = nullptr);
    ~ManageCoursesDialog();

    void setCourseData(const QString &name, const QString &type, const QString &fieldOfStudy, const QString &semester);
    QString getGroupNumber() const;
    QString getTeacherId() const;

private slots:
    void onSave();

private:
    Ui::ManageCoursesDialog *ui;
};

#endif // MANAGECOURSESDIALOG_H
