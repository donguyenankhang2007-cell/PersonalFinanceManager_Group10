#ifndef ACCOUNTREPOSITORY_H
#define ACCOUNTREPOSITORY_H

#include <QVector>

#include "../models/Account.h"

class AccountRepository
{
public:
    AccountRepository();

    bool addAccount(const Account &account);

    bool updateAccount(const Account &account);

    bool deleteAccount(int id);

    QVector<Account> getAllAccounts();

    Account getAccountById(int id);
};

#endif // ACCOUNTREPOSITORY_H
