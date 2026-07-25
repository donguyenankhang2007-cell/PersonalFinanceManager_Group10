#include "Account.h"

Account::Account()
    : id(0),
      name(""),
      balance(0.0),
      description("")
{
}

Account::Account(int id,
                 const QString& name,
                 double balance,
                 const QString& description)
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

void Account::setName(const QString& name)
{
    this->name = name;
}

void Account::setBalance(double balance)
{
    this->balance = balance;
}

void Account::setDescription(const QString& description)
{
    this->description = description;
}

void Account::deposit(double amount)
{
    if (amount > 0)
        balance += amount;
}

bool Account::withdraw(double amount)
{
    if (amount <= 0)
        return false;

    if (amount > balance)
        return false;

    balance -= amount;
    return true;
}

QString Account::toString() const
{
    return QString("%1 | Balance: %2")
            .arg(name)
            .arg(balance);
}