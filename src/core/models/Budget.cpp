#include "Budget.h"

Budget::Budget()
    : id(0),
    categoryId(0),
    amount(0.0),
    month(1),
    year(2000)
{
}

Budget::Budget(int id,
               int categoryId,
               double amount,
               int month,
               int year)
    : id(id),
    categoryId(categoryId),
    amount(amount),
    month(month),
    year(year)
{
}

int Budget::getId() const
{
    return id;
}

int Budget::getCategoryId() const
{
    return categoryId;
}

double Budget::getAmount() const
{
    return amount;
}

int Budget::getMonth() const
{
    return month;
}

int Budget::getYear() const
{
    return year;
}

void Budget::setId(int id)
{
    this->id = id;
}

void Budget::setCategoryId(int categoryId)
{
    this->categoryId = categoryId;
}

void Budget::setAmount(double amount)
{
    this->amount = amount;
}

void Budget::setMonth(int month)
{
    this->month = month;
}

void Budget::setYear(int year)
{
    this->year = year;
}

QString Budget::toString() const
{
    return QString("Budget [%1] | Category: %2 | Amount: %3 | %4/%5")
            .arg(id)
            .arg(categoryId)
            .arg(amount, 0, 'f', 0)
            .arg(month)
            .arg(year);
}