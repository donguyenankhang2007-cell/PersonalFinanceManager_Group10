#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>

class DatabaseManager
{
public:
    DatabaseManager();

    static DatabaseManager& instance();

    bool openDatabase();

    bool initializeDatabase();

    void closeDatabase();

    QSqlDatabase database();

private:
    QSqlDatabase db;
};

#endif