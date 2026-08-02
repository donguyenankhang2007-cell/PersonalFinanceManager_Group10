#include "TransactionRepository.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "../database/DatabaseManager.h"

TransactionRepository::TransactionRepository()
{
}

int TransactionRepository::addTransaction(const Transaction &transaction)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "INSERT INTO Transactions(accountId, categoryId, amount, transactionDate, note, type) "
        "VALUES(:accountId, :categoryId, :amount, :transactionDate, :note, :type)");

    query.bindValue(":accountId", transaction.getAccountId());
    query.bindValue(":categoryId", transaction.getCategoryId());
    query.bindValue(":amount", transaction.getAmount());
    query.bindValue(":transactionDate",
                    transaction.getTransactionDate().toString(Qt::ISODate));
    query.bindValue(":note", transaction.getNote());
    query.bindValue(":type", transaction.getType());

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
        return 0;
    }

    return query.lastInsertId().toInt();
}

bool TransactionRepository::updateTransaction(const Transaction &transaction)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "UPDATE Transactions "
        "SET accountId = :accountId, "
        "categoryId = :categoryId, "
        "amount = :amount, "
        "transactionDate = :transactionDate, "
        "note = :note, "
        "type = :type "
        "WHERE id = :id");

    query.bindValue(":id", transaction.getId());
    query.bindValue(":accountId", transaction.getAccountId());
    query.bindValue(":categoryId", transaction.getCategoryId());
    query.bindValue(":amount", transaction.getAmount());
    query.bindValue(":transactionDate",
                    transaction.getTransactionDate().toString(Qt::ISODate));
    query.bindValue(":note", transaction.getNote());
    query.bindValue(":type", transaction.getType());

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

bool TransactionRepository::deleteTransaction(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "DELETE FROM Transactions WHERE id = :id");

    query.bindValue(":id", id);

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

QVector<Transaction> TransactionRepository::getAllTransactions()
{
    QVector<Transaction> transactions;

    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare("SELECT * FROM Transactions");

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
        return transactions;
    }

    while(query.next())
    {
        Transaction transaction(
            query.value("id").toInt(),
            query.value("accountId").toInt(),
            query.value("categoryId").toInt(),
            query.value("amount").toDouble(),
            QDate::fromString(
                query.value("transactionDate").toString(),
                Qt::ISODate),
            query.value("note").toString(),
            query.value("type").toString()
            );

        transactions.push_back(transaction);
    }

    return transactions;
}

Transaction TransactionRepository::getTransactionById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "SELECT * FROM Transactions WHERE id = :id");

    query.bindValue(":id", id);

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
        return Transaction();
    }

    if(query.next())
    {
        return Transaction(
            query.value("id").toInt(),
            query.value("accountId").toInt(),
            query.value("categoryId").toInt(),
            query.value("amount").toDouble(),
            QDate::fromString(
                query.value("transactionDate").toString(),
                Qt::ISODate),
            query.value("note").toString(),
            query.value("type").toString()
            );
    }

    return Transaction();
}