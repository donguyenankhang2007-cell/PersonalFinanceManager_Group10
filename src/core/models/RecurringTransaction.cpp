#include "RecurringTransaction.h"

RecurringTransaction::RecurringTransaction()
    : BaseModel()
    , note("")
    , amount(0)
    , type("expense")
    , accountId(0)
    , categoryId(0)
    , frequency("monthly")
    , nextDate(QDate::currentDate())
    , endDate()
    , active(true)
{
}

RecurringTransaction::RecurringTransaction(int id,
                                           const QString& note,
                                           double amount,
                                           const QString& type,
                                           int accountId,
                                           int categoryId,
                                           const QString& frequency,
                                           const QDate& nextDate,
                                           const QDate& endDate,
                                           bool active)
    : BaseModel(id)
    , note(note)
    , amount(amount)
    , type(type)
    , accountId(accountId)
    , categoryId(categoryId)
    , frequency(frequency)
    , nextDate(nextDate)
    , endDate(endDate)
    , active(active)
{
}

QString RecurringTransaction::getNote() const { return note; }
double RecurringTransaction::getAmount() const { return amount; }
QString RecurringTransaction::getType() const { return type; }
int RecurringTransaction::getAccountId() const { return accountId; }
int RecurringTransaction::getCategoryId() const { return categoryId; }
QString RecurringTransaction::getFrequency() const { return frequency; }
QDate RecurringTransaction::getNextDate() const { return nextDate; }
QDate RecurringTransaction::getEndDate() const { return endDate; }

bool RecurringTransaction::hasEndDate() const { return endDate.isValid(); }
bool RecurringTransaction::isActive() const { return active; }

void RecurringTransaction::setNote(const QString& value) { note = value; }
void RecurringTransaction::setAmount(double value) { amount = value; }
void RecurringTransaction::setType(const QString& value) { type = value; }
void RecurringTransaction::setAccountId(int value) { accountId = value; }
void RecurringTransaction::setCategoryId(int value) { categoryId = value; }
void RecurringTransaction::setFrequency(const QString& value) { frequency = value; }
void RecurringTransaction::setNextDate(const QDate& date) { nextDate = date; }
void RecurringTransaction::setEndDate(const QDate& date) { endDate = date; }
void RecurringTransaction::setActive(bool value) { active = value; }

QString RecurringTransaction::frequencyDisplay() const
{
    if (frequency == "daily")
        return "Hàng ngày";
    if (frequency == "weekly")
        return "Hàng tuần";
    if (frequency == "yearly")
        return "Hàng năm";
    return "Hàng tháng";
}

QString RecurringTransaction::toString() const
{
    return QString("Recurring [%1]: %2 - %3 VND (%4)")
        .arg(m_id)
        .arg(note)
        .arg(amount)
        .arg(frequencyDisplay());
}
