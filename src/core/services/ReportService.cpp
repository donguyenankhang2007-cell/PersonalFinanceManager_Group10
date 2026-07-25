#include "ReportService.h"

ReportService::ReportService()
{
}

double ReportService::getTotalIncome(const QList<Transaction>& transactions,
                                      const QDate& fromDate,
                                      const QDate& toDate) const
{
    double total = 0.0;
    for (const Transaction& t : transactions) {
        bool inRange = (t.getDate() >= fromDate && t.getDate() <= toDate);
        if (inRange && t.getType() == "income") {
            total += t.getAmount();
        }
    }
    return total;
}

double ReportService::getTotalExpense(const QList<Transaction>& transactions,
                                       const QDate& fromDate,
                                       const QDate& toDate) const
{
    double total = 0.0;
    for (const Transaction& t : transactions) {
        bool inRange = (t.getDate() >= fromDate && t.getDate() <= toDate);
        if (inRange && t.getType() == "expense") {
            total += t.getAmount();
        }
    }
    return total;
}

Report ReportService::generateReport(const QList<Transaction>& transactions,
                                      const QDate& fromDate,
                                      const QDate& toDate) const
{
    double income = getTotalIncome(transactions, fromDate, toDate);
    double expense = getTotalExpense(transactions, fromDate, toDate);
    return Report(income, expense, income - expense);
}

double ReportService::compareExpense(const QList<Transaction>& transactions,
                                      const QDate& currentFrom, const QDate& currentTo,
                                      const QDate& previousFrom, const QDate& previousTo) const
{
    double current = getTotalExpense(transactions, currentFrom, currentTo);
    double previous = getTotalExpense(transactions, previousFrom, previousTo);

    if (previous == 0.0) {
        return (current == 0.0) ? 0.0 : 100.0;
    }
    return ((current - previous) / previous) * 100.0;
}