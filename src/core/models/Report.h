#ifndef REPORT_H
#define REPORT_H

#include <string>
#include <vector>
#include <map>
#include "Transaction.h"
#include "Budget.h"

class Report {
public:
    // Ném std::invalid_argument nếu month sai định dạng "YYYY-MM"
    Report(const std::vector<Transaction>& transactions, const std::vector<Budget>& budgets);

    double getTotalIncome(const std::string& month) const;
    double getTotalExpense(const std::string& month) const;
    double getBalance(const std::string& month) const;

    // map: tên category -> tổng chi tiêu trong tháng, chỉ gồm category có phát sinh chi tiêu
    std::map<std::string, double> getExpenseByCategory(const std::string& month) const;

    // % thay đổi chi tiêu so với tháng trước.
    // Nếu tháng trước không có chi tiêu (previous == 0):
    //   - current cũng == 0 -> trả về 0.0 (không thay đổi)
    //   - current > 0       -> trả về 100.0 (coi như tăng 100%, tránh chia cho 0)
    double compareWithPreviousMonth(const std::string& currentMonth, const std::string& previousMonth) const;

    // Danh sách category đang vượt ngân sách trong tháng
    std::vector<std::string> getOverBudgetCategories(const std::string& month) const;

private:
    std::vector<Transaction> m_transactions;
    std::vector<Budget> m_budgets;

    void validateMonth(const std::string& month) const;
};

#endif