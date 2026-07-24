#include "Report.h"
#include <stdexcept>

Report::Report(const std::vector<Transaction>& transactions, const std::vector<Budget>& budgets)
    : m_transactions(transactions), m_budgets(budgets) {}

void Report::validateMonth(const std::string& month) const {
    if (!Budget::isValidMonthFormat(month)) {
        throw std::invalid_argument("Month phai dung dinh dang YYYY-MM");
    }
}

double Report::getTotalIncome(const std::string& month) const {
    validateMonth(month);
    double total = 0.0;
    for (const auto& t : m_transactions) {
        if (t.getMonthKey() == month && t.getType() == TransactionType::Income) {
            total += t.getAmount();
        }
    }
    return total;
}

double Report::getTotalExpense(const std::string& month) const {
    validateMonth(month);
    double total = 0.0;
    for (const auto& t : m_transactions) {
        if (t.getMonthKey() == month && t.getType() == TransactionType::Expense) {
            total += t.getAmount();
        }
    }
    return total;
}

double Report::getBalance(const std::string& month) const {
    // getTotalIncome/getTotalExpense đã tự validate month
    return getTotalIncome(month) - getTotalExpense(month);
}

std::map<std::string, double> Report::getExpenseByCategory(const std::string& month) const {
    validateMonth(month);
    std::map<std::string, double> result;
    for (const auto& t : m_transactions) {
        if (t.getMonthKey() == month && t.getType() == TransactionType::Expense) {
            result[t.getCategoryName()] += t.getAmount();
        }
    }
    return result;
}

double Report::compareWithPreviousMonth(const std::string& currentMonth, const std::string& previousMonth) const {
    validateMonth(currentMonth);
    validateMonth(previousMonth);

    double current = getTotalExpense(currentMonth);
    double previous = getTotalExpense(previousMonth);

    if (previous == 0.0) {
        return (current == 0.0) ? 0.0 : 100.0;
    }
    return ((current - previous) / previous) * 100.0;
}

std::vector<std::string> Report::getOverBudgetCategories(const std::string& month) const {
    validateMonth(month);
    std::vector<std::string> result;
    for (const auto& b : m_budgets) {
        if (b.getMonth() == month && b.isOverBudget(m_transactions)) {
            result.push_back(b.getCategoryName());
        }
    }
    return result;
}