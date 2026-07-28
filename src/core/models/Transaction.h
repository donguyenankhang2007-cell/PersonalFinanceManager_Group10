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
    
    QDate date;
    QString note;
    QString type;
    QDate transactionDate;

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
                const QDate& transactionDate,
                const QString& note);

    int getId() const;
    int getAccountId() const;
    int getCategoryId() const;
    double getAmount() const;
    QDate getDate() const;
    QString getNote() const;
    QString getType() const;
    QDate getTransactionDate() const;

    void setId(int id);
    void setAccountId(int accountId);
    void setCategoryId(int categoryId);
    void setAmount(double amount);
    void setDate(const QDate& date);
    void setNote(const QString& note);
    void setType(const QString& type);
    QString toString() const override;
    void setTransactionDate(const QDate& transactionDate);
};

#endif
