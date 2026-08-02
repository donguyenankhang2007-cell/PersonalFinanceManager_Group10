#ifndef RECURRINGTRANSACTION_H
#define RECURRINGTRANSACTION_H

#include <QString>
#include <QDate>
#include "BaseModel.h"

// Giao dịch định kỳ: tự sinh giao dịch theo chu kỳ
// (hằng ngày / hằng tuần / hằng tháng / hằng năm)
class RecurringTransaction : public BaseModel
{
private:
    QString note;
    double amount;
    QString type;          // "income" hoặc "expense" (lấy từ category)
    int accountId;
    int categoryId;
    QString frequency;     // "daily" | "weekly" | "monthly" | "yearly"
    QDate nextDate;        // lần giao dịch tiếp theo
    QDate endDate;         // không hợp lệ (invalid) = không có hạn
    bool active;

public:
    RecurringTransaction();

    RecurringTransaction(int id,
                         const QString& note,
                         double amount,
                         const QString& type,
                         int accountId,
                         int categoryId,
                         const QString& frequency,
                         const QDate& nextDate,
                         const QDate& endDate,
                         bool active);

    ~RecurringTransaction() = default;

    QString getNote() const;
    double getAmount() const;
    QString getType() const;
    int getAccountId() const;
    int getCategoryId() const;
    QString getFrequency() const;
    QDate getNextDate() const;
    QDate getEndDate() const;
    bool hasEndDate() const;
    bool isActive() const;

    void setNote(const QString& note);
    void setAmount(double amount);
    void setType(const QString& type);
    void setAccountId(int accountId);
    void setCategoryId(int categoryId);
    void setFrequency(const QString& frequency);
    void setNextDate(const QDate& date);
    void setEndDate(const QDate& date);
    void setActive(bool active);

    QString frequencyDisplay() const;

    QString toString() const override;
    QString getDisplayName() const override { return note; }
};

#endif // RECURRINGTRANSACTION_H
