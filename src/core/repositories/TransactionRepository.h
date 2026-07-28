#ifndef TRANSACTIONREPOSITORY_H
#define TRANSACTIONREPOSITORY_H

#include <QVector>

#include "../models/Transaction.h"

class TransactionRepository
{
public:
    TransactionRepository();

    bool addTransaction(const Transaction &transaction);

    bool updateTransaction(const Transaction &transaction);

    bool deleteTransaction(int id);

    QVector<Transaction> getAllTransactions();

    Transaction getTransactionById(int id);
};

#endif // TRANSACTIONREPOSITORY_H