#ifndef SEARCHUSERDIALOG_H
#define SEARCHUSERDIALOG_H

#include <QDialog>

namespace Ui {
class SearchUserDialog;
}

class SearchUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SearchUserDialog(QWidget *parent = nullptr);
    ~SearchUserDialog();
    void setUserData(const QString& id, const QString& firstName, const QString& secondName,
                     const QString& email, const QString& additionalInfo);

private:
    Ui::SearchUserDialog *ui;
};

#endif // SEARCHUSERDIALOG_H
