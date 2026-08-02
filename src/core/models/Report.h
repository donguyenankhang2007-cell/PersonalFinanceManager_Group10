#ifndef REPORT_H
#define REPORT_H

#include <QString>
#include "BaseModel.h"

class Report : public BaseModel
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

    QString toString() const override;
    QString getDisplayName() const override { return "Report"; }
};

#endif // REPORT_H
