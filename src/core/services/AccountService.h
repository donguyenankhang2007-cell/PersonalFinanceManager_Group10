#ifndef ACCOUNTSERVICE_H
#define ACCOUNTSERVICE_H

#include <QObject>
#include <QVector>
#include "../models/Account.h"
#include "../repositories/AccountRepository.h"

class AccountService : public QObject
{
    Q_OBJECT

public:
    explicit AccountService(QObject* parent = nullptr);

    bool addAccount(const Account& account, QString* errorMessage = nullptr);
    bool updateAccount(const Account& account, QString* errorMessage = nullptr);
    bool removeAccount(int id, QString* errorMessage = nullptr);

    QVector<Account> getAllAccounts();
    Account getAccountById(int id);

signals:
    void accountAdded(const Account& account);
    void accountUpdated(const Account& account);
    void accountRemoved(int accountId);

private:
    AccountRepository m_accountRepo;
};

#endif // ACCOUNTSERVICE_H
