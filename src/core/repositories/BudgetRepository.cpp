#include "BudgetRepository.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "../database/DatabaseManager.h"

BudgetRepository::BudgetRepository()
{
}

bool BudgetRepository::addBudget(const Budget &budget)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "INSERT INTO Budget(categoryId, amount, month, year) "
        "VALUES(:categoryId, :amount, :month, :year)");

    query.bindValue(":categoryId", budget.getCategoryId());
    query.bindValue(":amount", budget.getAmount());
    query.bindValue(":month", budget.getMonth());
    query.bindValue(":year", budget.getYear());

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

bool BudgetRepository::updateBudget(const Budget &budget)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "UPDATE Budget "
        "SET categoryId = :categoryId, "
        "amount = :amount, "
        "month = :month, "
        "year = :year "
        "WHERE id = :id");

    query.bindValue(":id", budget.getId());
    query.bindValue(":categoryId", budget.getCategoryId());
    query.bindValue(":amount", budget.getAmount());
    query.bindValue(":month", budget.getMonth());
    query.bindValue(":year", budget.getYear());

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

bool BudgetRepository::deleteBudget(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "DELETE FROM Budget WHERE id = :id");

    query.bindValue(":id", id);

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

QVector<Budget> BudgetRepository::getAllBudgets()
{
    QVector<Budget> budgets;

    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare("SELECT * FROM Budget");

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
        return budgets;
    }

    while(query.next())
    {
        Budget budget(
            query.value("id").toInt(),
            query.value("categoryId").toInt(),
            query.value("amount").toDouble(),
            query.value("month").toInt(),
            query.value("year").toInt()
            );

        budgets.push_back(budget);
    }

    return budgets;
}

Budget BudgetRepository::getBudgetById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "SELECT * FROM Budget WHERE id = :id");

    query.bindValue(":id", id);

    if(!query.exec())
    {
        qDebug() << query.lastError().text();
        return Budget();
    }

    if(query.next())
    {
        return Budget(
            query.value("id").toInt(),
            query.value("categoryId").toInt(),
            query.value("amount").toDouble(),
            query.value("month").toInt(),
            query.value("year").toInt()
            );
    }

    return Budget();
}