#include "Transaction.h"
#include <cctype>

bool Transaction::isValidDateFormat(const std::string& date) {
    // Kiểm tra đúng 10 ký tự dạng YYYY-MM-DD
    if (date.size() != 10) return false;
    if (date[4] != '-' || date[7] != '-') return false;

    for (int i = 0; i < 10; ++i) {
        if (i == 4 || i == 7) continue;
        if (!std::isdigit(static_cast<unsigned char>(date[i]))) return false;
    }

    int month = std::stoi(date.substr(5, 2));
    int day = std::stoi(date.substr(8, 2));

    if (month < 1 || month > 12) return false;
    if (day < 1 || day > 31) return false;

    return true;
}

void Transaction::validateAmount(double amount) const {
    if (amount <= 0) {
        throw std::invalid_argument("Amount phai lon hon 0");
    }
}

void Transaction::validateDate(const std::string& date) const {
    if (!isValidDateFormat(date)) {
        throw std::invalid_argument("Date phai dung dinh dang YYYY-MM-DD");
    }
}

Transaction::Transaction(double amount, const std::string& date,
                          const std::string& categoryName, TransactionType type,
                          const std::string& note)
    : m_amount(amount), m_date(date), m_categoryName(categoryName),
      m_type(type), m_note(note) {
    validateAmount(amount);
    validateDate(date);

    if (categoryName.empty()) {
        throw std::invalid_argument("Category khong duoc de trong");
    }
}

double Transaction::getAmount() const { return m_amount; }
std::string Transaction::getDate() const { return m_date; }

std::string Transaction::getMonthKey() const {
    return m_date.substr(0, 7); // "YYYY-MM"
}

std::string Transaction::getCategoryName() const { return m_categoryName; }
TransactionType Transaction::getType() const { return m_type; }
std::string Transaction::getNote() const { return m_note; }

void Transaction::setAmount(double amount) {
    validateAmount(amount);
    m_amount = amount;
}

void Transaction::setNote(const std::string& note) { m_note = note; }