#include "Transaction.h"

Transaction::Transaction()
    : BaseModel(),
      accountId(0),
      categoryId(0),
      amount(0.0),
      transactionDate(QDate::currentDate()),
      note(""),
      type("")
{
}

Transaction::Transaction(int id,
                         int accountId,
                         int categoryId,
                         double amount,
                         const QDate& date,
                         const QString& note,
                         const QString& type)
    : BaseModel(id),
      accountId(accountId),
      categoryId(categoryId),
      amount(amount),
      transactionDate(date),
      note(note),
      type(type)
{
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

QDate Transaction::getDate() const
{
    return transactionDate;
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
    this->transactionDate = date;
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
            .arg(transactionDate.toString("dd/MM/yyyy"))
            .arg(amount)
            .arg(note);
}
