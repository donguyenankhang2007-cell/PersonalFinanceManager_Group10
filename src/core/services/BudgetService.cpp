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
    // getAmount() là hạn mức ngân sách (limitAmount) của Budget
    return spent > budget.getAmount();
}

double BudgetService::getUsagePercent(const Budget& budget,
                                       int categoryId,
                                       const QList<Transaction>& transactions) const
{
    // getAmount() là hạn mức ngân sách
    if (budget.getAmount() == 0.0) {
        return 0.0;
    }
    double spent = calculateSpent(budget, categoryId, transactions);
    return spent / budget.getAmount();
}

// Tính ra số tiền đã chi thực tế cho budget này
// Vì Budget model không có field spentAmount riêng,
// hàm này trả về double thay vì Budget để tránh nhầm lẫn.
// Caller tự dùng giá trị này để hiển thị hoặc so sánh.
double BudgetService::getSpentAmount(const Budget& budget,
                                      int categoryId,
                                      const QList<Transaction>& transactions) const
{
    return calculateSpent(budget, categoryId, transactions);
}