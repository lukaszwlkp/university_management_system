#ifndef MYGRADESPAGE_H
#define MYGRADESPAGE_H

#include <QWidget>
#include <QMap>

namespace Ui {
class MyGradesPage;
}

class MyGradesPage : public QWidget
{
    Q_OBJECT

public:
    explicit MyGradesPage(const QString& loggedUserId, QWidget *parent = nullptr);
    ~MyGradesPage();

signals:
    void backToMenu();

protected:
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void on_pushButton_return_clicked();

private:
    Ui::MyGradesPage *ui;
    QString loggedUserId;

    void setupTable();
    void loadGrades();
    QMap<int, QMap<QString, QMap<QString, QString>>> fetchGrades();
    void populateGradesTable(const QMap<int, QMap<QString, QMap<QString, QString>>>& gradesData);
};


#endif // MYGRADESPAGE_H

