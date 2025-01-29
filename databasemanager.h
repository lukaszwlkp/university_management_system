#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql/QSqlDatabase>

class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();
    bool connectDatabase();
    QSqlDatabase& getDatabase();

private:
    QSqlDatabase DBConnection;
};

#endif // DATABASEMANAGER_H
