#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <QString>

class Account
{
private:
    int id;
    QString name;
    double balance;
    QString description;

public:
    Account();

    Account(int id,
            const QString& name,
            double balance,
            const QString& description = "");

    ~Account() = default;

    int getId() const;
    QString getName() const;
    double getBalance() const;
    QString getDescription() const;

    void setId(int id);
    void setName(const QString& name);
    void setBalance(double balance);
    void setDescription(const QString& description);

    void deposit(double amount);
    bool withdraw(double amount);

    QString toString() const override;
};

#endif
