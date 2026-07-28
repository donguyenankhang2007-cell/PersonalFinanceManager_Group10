#include "AccountRepository.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "../database/DatabaseManager.h"

AccountRepository::AccountRepository()
{
}

bool AccountRepository::addAccount(const Account &account)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "INSERT INTO Account(name, balance, description) "
        "VALUES(:name, :balance, :description)");

    query.bindValue(":name", account.getName());
    query.bindValue(":balance", account.getBalance());
    query.bindValue(":description", account.getDescription());

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

QVector<Account> AccountRepository::getAllAccounts()
{
    QVector<Account> accounts;

    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare("SELECT * FROM Account");

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        return accounts;
    }

    while (query.next())
    {
        Account account(
            query.value("id").toInt(),
            query.value("name").toString(),
            query.value("balance").toDouble(),
            query.value("description").toString());

        accounts.push_back(account);
    }

    return accounts;
}

Account AccountRepository::getAccountById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "SELECT * FROM Account WHERE id = :id");

    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        return Account();
    }

    if (query.next())
    {
        return Account(
            query.value("id").toInt(),
            query.value("name").toString(),
            query.value("balance").toDouble(),
            query.value("description").toString());
    }

    return Account();
}

bool AccountRepository::updateAccount(const Account &account)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "UPDATE Account "
        "SET name = :name, "
        "balance = :balance, "
        "description = :description "
        "WHERE id = :id");

    query.bindValue(":id", account.getId());
    query.bindValue(":name", account.getName());
    query.bindValue(":balance", account.getBalance());
    query.bindValue(":description", account.getDescription());

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

bool AccountRepository::deleteAccount(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "DELETE FROM Account WHERE id = :id");

    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

