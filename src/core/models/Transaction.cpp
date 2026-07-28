#include "Transaction.h"

Transaction::Transaction()
    : id(0),
      accountId(0),
      categoryId(0),
      amount(0.0),
      transactionDate(QDate::currentDate()),
      note("")
{
}

Transaction::Transaction(int id,
                          int accountId,
                          int categoryId,
                          double amount,
                          const QDate& transactionDate,
                          const QString& note)
    : id(id),
      accountId(accountId),
      categoryId(categoryId),
      amount(amount),
      transactionDate(transactionDate),
      note(note)
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

void Transaction::setTransactionDate(const QDate& transactionDate)
{
    this->transactionDate = transactionDate;
}

void Transaction::setNote(const QString& note)
{
    this->note = note;
}
