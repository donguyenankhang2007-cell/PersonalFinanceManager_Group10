#include "Account.h"

Account::Account()
    : BaseModel(),
      name(""),
      balance(0.0),
      description("")
{
}

Account::Account(int id,
                 const QString &name,
                 double balance,
                 const QString &description)
    : BaseModel(id),
      name(name),
      balance(balance),
      description(description)
{
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

void Account::deposit(double amount)
{
    if (amount > 0) {
        balance += amount;
    }
}

bool Account::withdraw(double amount)
{
    if (amount > 0) {
        balance -= amount;
        return true;
    }
    return false;
}

QString Account::toString() const
{
    return QString("Account [%1] %2 | Balance: %3 | %4")
            .arg(m_id)
            .arg(name)
            .arg(balance, 0, 'f', 0)
            .arg(description);
}
