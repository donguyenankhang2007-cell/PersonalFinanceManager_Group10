#include "Budget.h"

Budget::Budget()
    : id(0),
      name(""),
      limitAmount(0.0),
      spentAmount(0.0),
      description("")
{
}

Budget::Budget(int id,
               const QString& name,
               double limitAmount,
               double spentAmount,
               const QString& description)
    : id(id),
      name(name),
      limitAmount(limitAmount),
      spentAmount(spentAmount),
      description(description)
{
}

int Budget::getId() const
{
    return id;
}

QString Budget::getName() const
{
    return name;
}

double Budget::getLimitAmount() const
{
    return limitAmount;
}

double Budget::getSpentAmount() const
{
    return spentAmount;
}

QString Budget::getDescription() const
{
    return description;
}

void Budget::setId(int id)
{
    this->id = id;
}

void Budget::setName(const QString& name)
{
    this->name = name;
}

void Budget::setLimitAmount(double limitAmount)
{
    this->limitAmount = limitAmount;
}

void Budget::setSpentAmount(double spentAmount)
{
    this->spentAmount = spentAmount;
}

void Budget::setDescription(const QString& description)
{
    this->description = description;
}

QString Budget::toString() const
{
    return QString("%1 | Limit: %2 | Spent: %3")
            .arg(name)
            .arg(limitAmount)
            .arg(spentAmount);
}