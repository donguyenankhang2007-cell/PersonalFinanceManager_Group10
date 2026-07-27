#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>

class DatabaseManager
{
public:
    static DatabaseManager& instance();

    bool openDatabase();

    bool initializeDatabase();

    void closeDatabase();

    QSqlDatabase database();

private:
    DatabaseManager();

    QSqlDatabase db;
};

#endif