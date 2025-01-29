#ifndef SEARCHUSERPAGE_H
#define SEARCHUSERPAGE_H

#include <QWidget>
#include <QKeyEvent>

namespace Ui {
class SearchUserPage;
}

class SearchUserPage : public QWidget
{
    Q_OBJECT

public:
    explicit SearchUserPage(const QString& loggedUserId, QWidget *parent = nullptr);
    ~SearchUserPage();

signals:
    void backToMenu();

private slots:
    void on_pushButton_return_clicked();
    void on_pushButton_search_clicked();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private:
    Ui::SearchUserPage *ui;
    QString loggedUserId;
};

#endif // SEARCHUSERPAGE_H
