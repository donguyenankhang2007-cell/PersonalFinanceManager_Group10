#include "BudgetService.h"

BudgetService::BudgetService(QObject* parent)
    : QObject(parent)
{
}

bool BudgetService::addBudget(const Budget& budget, QString* errorMessage)
{
    if (budget.getAmount() <= 0) {
        if (errorMessage) *errorMessage = "Hạn mức ngân sách phải lớn hơn 0";
        return false;
    }
    if (budget.getCategoryId() <= 0) {
        if (errorMessage) *errorMessage = "Ngân sách phải thuộc về 1 category hợp lệ";
        return false;
    }

    if (!m_budgetRepo.addBudget(budget)) {
        if (errorMessage) *errorMessage = "Không thể lưu ngân sách vào cơ sở dữ liệu";
        return false;
    }

    emit budgetAdded(budget);
    return true;
}

bool BudgetService::updateBudget(const Budget& budget, QString* errorMessage)
{
    if (budget.getId() <= 0) {
        if (errorMessage) *errorMessage = "Ngân sách cần có id hợp lệ";
        return false;
    }
    if (budget.getAmount() <= 0) {
        if (errorMessage) *errorMessage = "Hạn mức ngân sách phải lớn hơn 0";
        return false;
    }

    if (!m_budgetRepo.updateBudget(budget)) {
        if (errorMessage) *errorMessage = "Không thể cập nhật ngân sách";
        return false;
    }

    emit budgetUpdated(budget);
    return true;
}

bool BudgetService::removeBudget(int id, QString* errorMessage)
{
    if (id <= 0) {
        if (errorMessage) *errorMessage = "Ngân sách cần có id hợp lệ";
        return false;
    }

    if (!m_budgetRepo.deleteBudget(id)) {
        if (errorMessage) *errorMessage = "Không thể xóa ngân sách";
        return false;
    }

    emit budgetRemoved(id);
    return true;
}

QVector<Budget> BudgetService::getAllBudgets()
{
    return m_budgetRepo.getAllBudgets();
}

double BudgetService::calculateSpent(const Budget& budget,
                                     const QList<Transaction>& transactions) const
{
    double total = 0.0;
    for (const Transaction& t : transactions) {
        if (t.getCategoryId() == budget.getCategoryId()
            && t.isExpense()
            && t.getDate().month() == budget.getMonth()
            && t.getDate().year() == budget.getYear()) {
            total += t.getAmount();
        }
    }
    return total;
}

bool BudgetService::isOverBudget(const Budget& budget,
                                 const QList<Transaction>& transactions) const
{
    return calculateSpent(budget, transactions) > budget.getAmount();
}

double BudgetService::getUsagePercent(const Budget& budget,
                                      const QList<Transaction>& transactions) const
{
    if (budget.getAmount() == 0.0) {
        return 0.0;
    }
    return calculateSpent(budget, transactions) / budget.getAmount();
}

double BudgetService::getSpentAmount(const Budget& budget,
                                     const QList<Transaction>& transactions) const
{
    return calculateSpent(budget, transactions);
}
