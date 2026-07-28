#include "Account.h"

Account::Account()
{
    id = 0;
    name = "";
    balance = 0.0;
    description = "";
}

Account::Account(int id,
                 const QString &name,
                 double balance,
                 const QString &description)
    : id(id),
    name(name),
    balance(balance),
    description(description)
{
}

int Account::getId() const
{
    return id;
}

QString Account::getName() const
{
    return name;
}

double Account::getBalance() const
{
    return balance;
}

QString Account::getDescription() const
{
    return description;
}

void Account::setId(int id)
{
    this->id = id;
}

void Account::setName(const QString &name)
{
    this->name = name;
}

void Account::setBalance(double balance)
{
    this->balance = balance;
}

void Account::setDescription(const QString &description)
{
    this->description = description;
}