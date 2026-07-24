#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <stdexcept>

enum class TransactionType {
    Income,
    Expense
};

class Transaction {
public:
    // Ném std::invalid_argument nếu amount <= 0 hoặc date sai định dạng "YYYY-MM-DD"
    Transaction(double amount, const std::string& date,
                const std::string& categoryName, TransactionType type,
                const std::string& note = "");

    double getAmount() const;
    std::string getDate() const;      // "YYYY-MM-DD"
    std::string getMonthKey() const;  // "YYYY-MM", tiện cho Budget/Report lọc theo tháng
    std::string getCategoryName() const;
    TransactionType getType() const;
    std::string getNote() const;

    void setAmount(double amount);    // cũng validate, ném lỗi nếu <= 0
    void setNote(const std::string& note);

    // Kiểm tra định dạng "YYYY-MM-DD" hợp lệ (đúng số ký tự, tháng 1-12, ngày 1-31)
    // Dùng static để Budget/Report cũng có thể validate input tháng mà không cần tạo Transaction
    static bool isValidDateFormat(const std::string& date);

private:
    double m_amount;
    std::string m_date;
    std::string m_categoryName;
    TransactionType m_type;
    std::string m_note;

    void validateAmount(double amount) const;
    void validateDate(const std::string& date) const;
};

#endif