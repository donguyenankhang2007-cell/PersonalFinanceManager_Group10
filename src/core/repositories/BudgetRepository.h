#ifndef BUDGETREPOSITORY_H
#define BUDGETREPOSITORY_H

#include <QVector>

#include "../models/Budget.h"

class BudgetRepository
{
public:
    BudgetRepository();

    bool addBudget(const Budget &budget);

    bool updateBudget(const Budget &budget);

    bool deleteBudget(int id);

    QVector<Budget> getAllBudgets();

    Budget getBudgetById(int id);
};

#endif // BUDGETREPOSITORY_H