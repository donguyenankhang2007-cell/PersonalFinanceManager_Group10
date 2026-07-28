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

    // Cap nhat lai spentAmount cua budget dua tren danh sach transaction hien tai
    // Tra ve budget moi (khong sua truc tiep tham so dau vao)
    Budget refreshSpentAmount(const Budget& budget,
                               int categoryId,
                               const QList<Transaction>& transactions) const;
};

#endif // BUDGETSERVICE_H
