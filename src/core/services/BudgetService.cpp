#include "BudgetService.h"

BudgetService::BudgetService()
{
}

double BudgetService::calculateSpent(const Budget& budget,
                                      int categoryId,
                                      const QList<Transaction>& transactions) const
{
    Q_UNUSED(budget);
    double total = 0.0;
    for (const Transaction& t : transactions) {
        if (t.getCategoryId() == categoryId && t.getType() == "expense") {
            total += t.getAmount();
        }
    }
    return total;
}

bool BudgetService::isOverBudget(const Budget& budget,
                                  int categoryId,
                                  const QList<Transaction>& transactions) const
{
    double spent = calculateSpent(budget, categoryId, transactions);
    return spent > budget.getLimitAmount();
}

double BudgetService::getUsagePercent(const Budget& budget,
                                       int categoryId,
                                       const QList<Transaction>& transactions) const
{
    if (budget.getLimitAmount() == 0.0) {
        return 0.0;
    }
    double spent = calculateSpent(budget, categoryId, transactions);
    return spent / budget.getLimitAmount();
}

Budget BudgetService::refreshSpentAmount(const Budget& budget,
                                          int categoryId,
                                          const QList<Transaction>& transactions) const
{
    Budget updated = budget;
    double spent = calculateSpent(budget, categoryId, transactions);
    updated.setSpentAmount(spent);
    return updated;
}