#ifndef REPORT_H
#define REPORT_H

#include <QString>

class Report
{
private:
    double totalIncome;
    double totalExpense;
    double remainingBalance;

public:
    Report();

    Report(double totalIncome,
           double totalExpense,
           double remainingBalance);

    ~Report() = default;

    double getTotalIncome() const;
    double getTotalExpense() const;
    double getRemainingBalance() const;

    void setTotalIncome(double totalIncome);
    void setTotalExpense(double totalExpense);
    void setRemainingBalance(double remainingBalance);

    QString toString() const;
};

#endif // REPORT_H