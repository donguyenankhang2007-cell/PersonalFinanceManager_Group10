#include "Transaction.h"

Transaction::Transaction()
    : id(0),
      accountId(0),
      categoryId(0),
      amount(0.0),
      date(QDate::currentDate()),
      note(""),
      type(""),
      transactionDate(QDate::currentDate())
{
}

Transaction::Transaction(int id,
                         int accountId,
                         int categoryId,
                         double amount,
                         const QDate& date,
                         const QString& note,
                         const QString& type,
                          const QDate& transactionDate,
                          const QString& note)
    : id(id),
      accountId(accountId),
      categoryId(categoryId),
      amount(amount),
      date(date),
      note(note),
      type(type)
      transactionDate(transactionDate),
{
}

int Transaction::getId() const
{
    return id;
}

int Transaction::getAccountId() const
{
    return accountId;
}

int Transaction::getCategoryId() const
{
    return categoryId;
}

double Transaction::getAmount() const
{
    return amount;
}

QDate Transaction::getTransactionDate() const
{
    return transactionDate;
}

QString Transaction::getNote() const
{
    return note;
}

QString Transaction::getType() const
{
    return type;
}

void Transaction::setId(int id)
{
    this->id = id;
}

void Transaction::setAccountId(int accountId)
{
    this->accountId = accountId;
}

void Transaction::setCategoryId(int categoryId)
{
    this->categoryId = categoryId;
}

void Transaction::setAmount(double amount)
{
    this->amount = amount;
}

void Transaction::setDate(const QDate& date)
{
    this->date = date;
}
void Transaction::setTransactionDate(const QDate& transactionDate)
{
    this->transactionDate = transactionDate;
}

void Transaction::setNote(const QString& note)
{
    this->note = note;
}

void Transaction::setType(const QString& type)
{
    this->type = type;
}

QString Transaction::toString() const
{
    return QString("%1 | %2 | %3")
            .arg(date.toString("dd/MM/yyyy"))
            .arg(amount)
            .arg(note);
}
