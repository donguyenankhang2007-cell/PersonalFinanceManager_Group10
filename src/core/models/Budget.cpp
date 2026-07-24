#include "Budget.h"
#include <cctype>
#include <stdexcept>

bool Budget::isValidMonthFormat(const std::string& month) {
    if (month.size() != 7) return false;
    if (month[4] != '-') return false;

    for (int i = 0; i < 7; ++i) {
        if (i == 4) continue;
        if (!std::isdigit(static_cast<unsigned char>(month[i]))) return false;
    }

    int m = std::stoi(month.substr(5, 2));
    return m >= 1 && m <= 12;
}

void Budget::validateLimitAmount(double limitAmount) const {
    if (limitAmount <= 0) {
        throw std::invalid_argument("Limit amount phai lon hon 0");
    }
}

Budget::Budget(const std::string& categoryName, double limitAmount, const std::string& month)
    : m_categoryName(categoryName), m_limitAmount(limitAmount), m_month(month) {
    validateLimitAmount(limitAmount);

    if (categoryName.empty()) {
        throw std::invalid_argument("Category khong duoc de trong");
    }
    if (!isValidMonthFormat(month)) {
        throw std::invalid_argument("Month phai dung dinh dang YYYY-MM");
    }
}

std::string Budget::getCategoryName() const { return m_categoryName; }
double Budget::getLimitAmount() const { return m_limitAmount; }
std::string Budget::getMonth() const { return m_month; }

void Budget::setLimitAmount(double limitAmount) {
    validateLimitAmount(limitAmount);
    m_limitAmount = limitAmount;
}

double Budget::calculateSpent(const std::vector<Transaction>& transactions) const {
    double total = 0.0;
    for (const auto& t : transactions) {
        bool sameCategory = (t.getCategoryName() == m_categoryName);
        bool sameMonth = (t.getMonthKey() == m_month);
        bool isExpense = (t.getType() == TransactionType::Expense);

        if (sameCategory && sameMonth && isExpense) {
            total += t.getAmount();
        }
    }
    return total;
}

bool Budget::isOverBudget(const std::vector<Transaction>& transactions) const {
    return calculateSpent(transactions) > m_limitAmount;
}

double Budget::getUsagePercent(const std::vector<Transaction>& transactions) const {
    // m_limitAmount luôn > 0 nhờ validate ở constructor, nên không cần check chia cho 0
    return calculateSpent(transactions) / m_limitAmount;
}