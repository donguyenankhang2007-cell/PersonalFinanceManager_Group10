#ifndef BUDGETSERVICE_H
#define BUDGETSERVICE_H

#include <QObject>
#include <QList>
#include "../models/Budget.h"
#include "../models/Transaction.h"
#include "../repositories/BudgetRepository.h"

class BudgetService : public QObject
{
    Q_OBJECT

public:
    explicit BudgetService(QObject* parent = nullptr);

    // === CRUD ===
    bool addBudget(const Budget& budget, QString* errorMessage = nullptr);
    bool updateBudget(const Budget& budget, QString* errorMessage = nullptr);
    bool removeBudget(int id, QString* errorMessage = nullptr);

    QVector<Budget> getAllBudgets();

    // === Tính toán ngân sách ===
    // Tổng chi tiêu thực tế của budget (theo categoryId + tháng/năm của budget)
    double calculateSpent(const Budget& budget,
                          const QList<Transaction>& transactions) const;

    // Kiểm tra budget có bị vượt hạn mức không
    bool isOverBudget(const Budget& budget,
                      const QList<Transaction>& transactions) const;

    // % đã dùng, 0.0 - 1.0+ (có thể > 1.0 nếu vượt)
    double getUsagePercent(const Budget& budget,
                           const QList<Transaction>& transactions) const;

    double getSpentAmount(const Budget& budget,
                          const QList<Transaction>& transactions) const;

signals:
    void budgetAdded(const Budget& budget);
    void budgetUpdated(const Budget& budget);
    void budgetRemoved(int budgetId);

private:
    BudgetRepository m_budgetRepo;
};

#endif // BUDGETSERVICE_H
