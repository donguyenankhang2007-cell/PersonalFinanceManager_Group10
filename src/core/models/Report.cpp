#include "Report.h"

Report::Report()
    : totalIncome(0.0),
      totalExpense(0.0),
      remainingBalance(0.0)
{
}

Report::Report(double totalIncome,
               double totalExpense,
               double remainingBalance)
    : totalIncome(totalIncome),
      totalExpense(totalExpense),
      remainingBalance(remainingBalance)
{
}

double Report::getTotalIncome() const
{
    return totalIncome;
}

double Report::getTotalExpense() const
{
    return totalExpense;
}

double Report::getRemainingBalance() const
{
    return remainingBalance;
}

void Report::setTotalIncome(double totalIncome)
{
    this->totalIncome = totalIncome;
}

void Report::setTotalExpense(double totalExpense)
{
    this->totalExpense = totalExpense;
}

void Report::setRemainingBalance(double remainingBalance)
{
    this->remainingBalance = remainingBalance;
}

QString Report::toString() const
{
    return QString("Income: %1 | Expense: %2 | Remaining: %3")
            .arg(totalIncome)
            .arg(totalExpense)
            .arg(remainingBalance);
}