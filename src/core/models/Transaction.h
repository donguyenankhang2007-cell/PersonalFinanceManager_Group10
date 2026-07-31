#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <QString>
#include <QDate>
#include "BaseModel.h"

class Transaction : public BaseModel
{
private:
    int id;
    int accountId;
    int categoryId;
    double amount;
    QDate transactionDate;
    QDate date;
    QString note;
    QString type;

public:
    Transaction();

    Transaction(int id,
                int accountId,
                int categoryId,
                double amount,
                const QDate& transactionDate,
                const QString& note,
                const QString& type = "");

    ~Transaction() = default;

    int getId() const;
    int getAccountId() const;
    int getCategoryId() const;
    double getAmount() const;
    QDate getTransactionDate() const;
    QDate getDate() const;
    QString getNote() const;
    QString getType() const;

    void setId(int id);
    void setAccountId(int accountId);
    void setCategoryId(int categoryId);
    void setAmount(double amount);
    void setTransactionDate(const QDate& transactionDate);
    void setDate(const QDate& date);
    void setNote(const QString& note);
    void setType(const QString& type);

    QString toString() const override;
};

#endif // TRANSACTION_H
