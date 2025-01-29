#include "myschedulepage.h"
#include "ui_myschedulepage.h"

MySchedulePage::MySchedulePage(const QString& loggedUserId, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MySchedulePage),
    loggedUserId(loggedUserId)
{
    ui->setupUi(this);
}

MySchedulePage::~MySchedulePage()
{
    delete ui;
}

void MySchedulePage::on_pushButton_return_clicked()
{
    emit backToMenu();
}

void MySchedulePage::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        on_pushButton_return_clicked();
    }
}
