#ifndef BUDGET_H
#define BUDGET_H

#include <string>
#include <vector>
#include "Transaction.h"

class Budget {
public:
    // Ném std::invalid_argument nếu limitAmount <= 0 hoặc month sai định dạng "YYYY-MM"
    Budget(const std::string& categoryName, double limitAmount, const std::string& month);

    std::string getCategoryName() const;
    double getLimitAmount() const;
    std::string getMonth() const;

    void setLimitAmount(double limitAmount); // cũng validate

    // Tổng chi tiêu thực tế của category này trong tháng, dựa trên danh sách transaction truyền vào
    double calculateSpent(const std::vector<Transaction>& transactions) const;

    // Vượt hạn mức chưa
    bool isOverBudget(const std::vector<Transaction>& transactions) const;

    // % đã dùng, 0.0 - 1.0+ (có thể > 1.0 nếu vượt)
    double getUsagePercent(const std::vector<Transaction>& transactions) const;

    // Kiểm tra định dạng "YYYY-MM" hợp lệ
    static bool isValidMonthFormat(const std::string& month);

private:
    std::string m_categoryName;
    double m_limitAmount;
    std::string m_month;

    void validateLimitAmount(double limitAmount) const;
};

#endif