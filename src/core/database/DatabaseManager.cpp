#include "DatabaseManager.h"
#include <QSqlError>
#include <QFile>
#include <QSqlQuery>
#include <QDebug>
#include <QCoreApplication>
#include <QDir>
#include <QFileInfo>

DatabaseManager::DatabaseManager()
{
}

DatabaseManager &DatabaseManager::instance()
{
    static DatabaseManager manager;
    return manager;
}

bool DatabaseManager::openDatabase()
{
    // Nếu đã có kết nối thì trả về luôn
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }

    // Lấy đường dẫn thư mục chứa file .exe
    QDir dir(QCoreApplication::applicationDirPath());

    // Quay về thư mục gốc project
    dir.cdUp();   // Desktop_Qt_xxx -> build
    dir.cdUp();   // build -> PersonalFinanceManager_Group10

    // Kiểm tra thư mục data có tồn tại không
    if (!dir.exists("data"))
    {
        qDebug() << "Error: Cannot find 'data' directory!";
        return false;
    }

    // Đường dẫn đến file database
    QString databasePath = dir.filePath("data/finance.db");

    db.setDatabaseName(databasePath);

    // Mở database
    if (!db.open())
    {
        qDebug() << "Open database failed!";
        qDebug() << db.lastError().text();
        return false;
    }

    qDebug() << "Database opened successfully.";

    return true;
}

bool DatabaseManager::initializeDatabase()
{
    QFile file(":/database_schema.sql");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Cannot open database_schema.sql";
        return false;
    }

    QString sql = file.readAll();
    file.close();

    QStringList statements = sql.split(';', Qt::SkipEmptyParts);

    QSqlQuery query(db);

    for (const QString &statement : statements)
    {
        QString trimmed = statement.trimmed();

        if (trimmed.isEmpty())
            continue;

        if (!query.exec(trimmed))
        {
            qDebug() << "Database initialization failed:";
            qDebug() << query.lastError().text();
            return false;
        }
    }

    qDebug() << "Database initialized successfully.";

    return true;
}

void DatabaseManager::closeDatabase()
{
    db.close();
}

QSqlDatabase DatabaseManager::database()
{
    return db;
}