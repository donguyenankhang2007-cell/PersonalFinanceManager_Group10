#include "RecurringTransactionRepository.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDate>
#include <QDebug>

#include "../database/DatabaseManager.h"

RecurringTransactionRepository::RecurringTransactionRepository()
{
}

namespace {
RecurringTransaction mapRow(const QSqlQuery& query)
{
    QDate endDate = QDate::fromString(query.value("endDate").toString(),
                                      Qt::ISODate);
    return RecurringTransaction(
        query.value("id").toInt(),
        query.value("note").toString(),
        query.value("amount").toDouble(),
        query.value("type").toString(),
        query.value("accountId").toInt(),
        query.value("categoryId").toInt(),
        query.value("frequency").toString(),
        QDate::fromString(query.value("nextDate").toString(), Qt::ISODate),
        endDate,
        query.value("active").toInt() != 0);
}
}

int RecurringTransactionRepository::add(const RecurringTransaction& recurring)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "INSERT INTO RecurringTransactions"
        "(note, amount, type, accountId, categoryId, frequency, nextDate, endDate, active) "
        "VALUES(:note, :amount, :type, :accountId, :categoryId, "
        ":frequency, :nextDate, :endDate, :active)");

    query.bindValue(":note", recurring.getNote());
    query.bindValue(":amount", recurring.getAmount());
    query.bindValue(":type", recurring.getType());
    query.bindValue(":accountId", recurring.getAccountId());
    query.bindValue(":categoryId", recurring.getCategoryId());
    query.bindValue(":frequency", recurring.getFrequency());
    query.bindValue(":nextDate",
                    recurring.getNextDate().toString(Qt::ISODate));
    query.bindValue(":endDate",
                    recurring.hasEndDate()
                        ? recurring.getEndDate().toString(Qt::ISODate)
                        : QVariant());
    query.bindValue(":active", recurring.isActive() ? 1 : 0);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return 0;
    }
    return query.lastInsertId().toInt();
}

bool RecurringTransactionRepository::update(const RecurringTransaction& recurring)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "UPDATE RecurringTransactions "
        "SET note = :note, amount = :amount, type = :type, "
        "accountId = :accountId, categoryId = :categoryId, "
        "frequency = :frequency, nextDate = :nextDate, "
        "endDate = :endDate, active = :active "
        "WHERE id = :id");

    query.bindValue(":id", recurring.getId());
    query.bindValue(":note", recurring.getNote());
    query.bindValue(":amount", recurring.getAmount());
    query.bindValue(":type", recurring.getType());
    query.bindValue(":accountId", recurring.getAccountId());
    query.bindValue(":categoryId", recurring.getCategoryId());
    query.bindValue(":frequency", recurring.getFrequency());
    query.bindValue(":nextDate",
                    recurring.getNextDate().toString(Qt::ISODate));
    query.bindValue(":endDate",
                    recurring.hasEndDate()
                        ? recurring.getEndDate().toString(Qt::ISODate)
                        : QVariant());
    query.bindValue(":active", recurring.isActive() ? 1 : 0);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }
    return true;
}

bool RecurringTransactionRepository::remove(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("DELETE FROM RecurringTransactions WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }
    return true;
}

QVector<RecurringTransaction> RecurringTransactionRepository::getAll()
{
    QVector<RecurringTransaction> result;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM RecurringTransactions ORDER BY nextDate");

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return result;
    }

    while (query.next())
        result.push_back(mapRow(query));
    return result;
}

QVector<RecurringTransaction> RecurringTransactionRepository::getActive()
{
    QVector<RecurringTransaction> result;
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare(
        "SELECT * FROM RecurringTransactions WHERE active = 1 ORDER BY nextDate");

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return result;
    }

    while (query.next())
        result.push_back(mapRow(query));
    return result;
}

RecurringTransaction RecurringTransactionRepository::getById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());
    query.prepare("SELECT * FROM RecurringTransactions WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return RecurringTransaction();
    }

    if (query.next())
        return mapRow(query);
    return RecurringTransaction();
}
