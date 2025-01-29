#include "databasemanager.h"
#include <QCoreApplication>
#include <QSqlError>
#include <QDebug>

DatabaseManager::DatabaseManager() {}

DatabaseManager::~DatabaseManager() {
    if (DBConnection.isOpen()) {
        DBConnection.close();
        qDebug() << "Database connection closed.";
    }
}

bool DatabaseManager::connectDatabase() {
    DBConnection = QSqlDatabase::addDatabase("QSQLITE");
    DBConnection.setDatabaseName(QCoreApplication::applicationDirPath() + "/dataBase.db");

    if (DBConnection.open()) {
        qDebug() << "Database connected at:" << DBConnection.databaseName();
        return true;
    } else {
        qDebug() << "Database connection failed:" << DBConnection.lastError().text();
        return false;
    }
}

QSqlDatabase& DatabaseManager::getDatabase() {
    return DBConnection;
}
