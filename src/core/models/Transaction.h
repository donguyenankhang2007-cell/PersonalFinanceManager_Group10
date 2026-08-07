#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QString>
#include <QDate>
#include "BaseModel.h"

class Transaction : public BaseModel
{
private:
    int accountId;
    int categoryId;
    double amount;

    QDate transactionDate;
    QString note;
    QString type;

public:
    Transaction();

    Transaction(int id,
                int accountId,
                int categoryId,
                double amount,
                const QDate& date,
                const QString& note,
                const QString& type);

    ~Transaction() = default;

    int getAccountId() const;
    int getCategoryId() const;
    double getAmount() const;
    QDate getDate() const;
    QDate getTransactionDate() const;
    QString getNote() const;
    QString getType() const;

    bool isIncome() const { return type == "income"; }
    bool isExpense() const { return type == "expense"; }

    void setAccountId(int accountId);
    void setCategoryId(int categoryId);
    void setAmount(double amount);
    void setDate(const QDate& date);
    void setTransactionDate(const QDate& transactionDate);
    void setNote(const QString& note);
    void setType(const QString& type);

    QString toString() const override;
    QString getDisplayName() const override { return note; }
};

#endif // TRANSACTION_H
