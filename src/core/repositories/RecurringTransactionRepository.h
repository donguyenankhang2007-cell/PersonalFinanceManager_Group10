#ifndef RECURRINGTRANSACTIONREPOSITORY_H
#define RECURRINGTRANSACTIONREPOSITORY_H

#include <QVector>
#include "../models/RecurringTransaction.h"

class RecurringTransactionRepository
{
public:
    RecurringTransactionRepository();

    int add(const RecurringTransaction& recurring);
    bool update(const RecurringTransaction& recurring);
    bool remove(int id);

    QVector<RecurringTransaction> getAll();
    QVector<RecurringTransaction> getActive();
    RecurringTransaction getById(int id);
};

#endif // RECURRINGTRANSACTIONREPOSITORY_H
