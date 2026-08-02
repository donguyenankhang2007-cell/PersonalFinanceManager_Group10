#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QSqlDatabase>
#include <QString>

class DatabaseManager
{
public:
    DatabaseManager();

    static DatabaseManager& instance();

    bool openDatabase();
    bool openDatabase(const QString& databasePath);

    bool initializeDatabase();

    void closeDatabase();

    QSqlDatabase database();

private:
    QSqlDatabase db;
};

#endif
