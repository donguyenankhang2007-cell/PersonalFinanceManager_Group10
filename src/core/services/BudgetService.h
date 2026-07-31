#ifndef BUDGETSERVICE_H
#define BUDGETSERVICE_H

#include <QList>
#include "../models/Budget.h"
#include "../models/Transaction.h"

class BudgetService
{
public:
    BudgetService();
    ~BudgetService() = default;

    // Tính tổng chi tiêu thực tế cho 1 budget, dựa trên category (categoryId)
    // và danh sách transaction được truyền vào (do noi goi lay tu Repository)
    double calculateSpent(const Budget& budget,
                           int categoryId,
                           const QList<Transaction>& transactions) const;

    // Kiem tra budget co bi vuot han muc khong
    bool isOverBudget(const Budget& budget,
                       int categoryId,
                       const QList<Transaction>& transactions) const;

    // % da dung, 0.0 - 1.0+ (co the > 1.0 neu vuot)
    double getUsagePercent(const Budget& budget,
                            int categoryId,
                            const QList<Transaction>& transactions) const;

    // Tinh tong tien da chi thuc te cho budget nay (tra ve double)
    // Vi Budget model khong co field spentAmount rieng,
    // ham nay tra ve gia tri double de caller tu xu ly (hien thi / so sanh)
    double getSpentAmount(const Budget& budget,
                          int categoryId,
                          const QList<Transaction>& transactions) const;
};

#endif // BUDGETSERVICE_H
