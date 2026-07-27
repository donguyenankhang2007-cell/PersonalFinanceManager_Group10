#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QString>
#include <QDate>

class Transaction
{
private:
    int id;
    int accountId;
    int categoryId;
    double amount;
    QDate transactionDate;
    QString note;

public:
    Transaction();

    Transaction(int id,
                int accountId,
                int categoryId,
                double amount,
                const QDate& transactionDate,
                const QString& note);

    int getId() const;
    int getAccountId() const;
    int getCategoryId() const;
    double getAmount() const;
    QDate getTransactionDate() const;
    QString getNote() const;

    void setId(int id);
    void setAccountId(int accountId);
    void setCategoryId(int categoryId);
    void setAmount(double amount);
    void setTransactionDate(const QDate& transactionDate);
    void setNote(const QString& note);
};

#endif
