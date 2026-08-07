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
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }

    // Lấy đường dẫn ứng dụng
    QDir dir(QCoreApplication::applicationDirPath());

    // Duyệt ngược lên để tìm thư mục gốc chứa data/ hoặc CMakeLists.txt
    QString dataDirPath;
    QDir searchDir = dir;
    for (int i = 0; i < 7; ++i)
    {
        if (searchDir.exists("data") || searchDir.exists("CMakeLists.txt"))
        {
            dataDirPath = searchDir.absoluteFilePath("data");
            break;
        }
        if (!searchDir.cdUp())
            break;
    }

    // Nếu vẫn không tìm thấy, tạo thư mục data ngay tại đường dẫn chạy
    if (dataDirPath.isEmpty())
    {
        dir.mkdir("data");
        dataDirPath = dir.absoluteFilePath("data");
    }
    else
    {
        QDir().mkpath(dataDirPath);
    }

    QString databasePath = QDir(dataDirPath).filePath("finance.db");
    return openDatabase(databasePath);
}

bool DatabaseManager::openDatabase(const QString& databasePath)
{
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
        db = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
    }

    db.setDatabaseName(databasePath);

    if (!db.open())
    {
        qDebug() << "Open database failed!";
        qDebug() << db.lastError().text();
        return false;
    }

    // Bật kiểm tra khóa ngoại (SQLite mặc định tắt)
    QSqlQuery fkQuery(db);
    fkQuery.exec("PRAGMA foreign_keys = ON");

    qDebug() << "Database opened successfully at:" << databasePath;
    return true;
}

bool DatabaseManager::initializeDatabase()
{
    QString sql;

    // Thử đọc từ Qt Resource trước
    QFile file(":/database_schema.sql");
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        sql = file.readAll();
        file.close();
    }
    else
    {
        // Fallback đọc trực tiếp từ đĩa
        QDir dir(QCoreApplication::applicationDirPath());
        QDir searchDir = dir;
        QString schemaPath;
        for (int i = 0; i < 7; ++i)
        {
            if (searchDir.exists("data/database_schema.sql"))
            {
                schemaPath = searchDir.absoluteFilePath("data/database_schema.sql");
                break;
            }
            if (!searchDir.cdUp())
                break;
        }

        QFile diskFile(schemaPath);
        if (diskFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            sql = diskFile.readAll();
            diskFile.close();
        }
        else
        {
            // Cấu hình bảng mặc định nếu không đọc được file
            sql = R"(
                CREATE TABLE IF NOT EXISTS Account (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, balance REAL DEFAULT 0, description TEXT);
                CREATE TABLE IF NOT EXISTS Category (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, type TEXT NOT NULL, color TEXT, icon TEXT);
                CREATE TABLE IF NOT EXISTS Transactions (id INTEGER PRIMARY KEY AUTOINCREMENT, accountId INTEGER NOT NULL, categoryId INTEGER NOT NULL, amount REAL NOT NULL, transactionDate TEXT NOT NULL, note TEXT, type TEXT NOT NULL DEFAULT 'expense');
                CREATE TABLE IF NOT EXISTS Budget (id INTEGER PRIMARY KEY AUTOINCREMENT, categoryId INTEGER NOT NULL, amount REAL NOT NULL, month INTEGER NOT NULL, year INTEGER NOT NULL);
                CREATE TABLE IF NOT EXISTS RecurringTransactions (id INTEGER PRIMARY KEY AUTOINCREMENT, note TEXT NOT NULL, amount REAL NOT NULL, type TEXT NOT NULL DEFAULT 'expense', accountId INTEGER NOT NULL, categoryId INTEGER NOT NULL, frequency TEXT NOT NULL DEFAULT 'monthly', nextDate TEXT NOT NULL, endDate TEXT, active INTEGER NOT NULL DEFAULT 1);
            )";
        }
    }

    QStringList statements = sql.split(';', Qt::SkipEmptyParts);
    QSqlQuery query(db);

    for (const QString &statement : statements)
    {
        QString trimmed = statement.trimmed();
        if (trimmed.isEmpty())
            continue;

        if (!query.exec(trimmed))
        {
            qDebug() << "Database statement execution warning/error:";
            qDebug() << query.lastError().text();
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
