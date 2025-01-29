#ifndef MANAGEUSERSDIALOG_H
#define MANAGEUSERSDIALOG_H

#include <QDialog>

namespace Ui {
class ManageUsersDialog;
}

class ManageUsersDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ManageUsersDialog(QWidget *parent = nullptr);
    ~ManageUsersDialog();
    void setUserData(const QString& id, const QString& firstName, const QString& secondName,
                     const QString& email, const QString& additionalInfo);

private slots:
    void onSave();

private:
    Ui::ManageUsersDialog *ui;

    bool isEmailAlreadyUsed(const QString& email);
    bool isIdAlreadyUsed(const QString& id);
};

#endif // MANAGEUSERSDIALOG_H
