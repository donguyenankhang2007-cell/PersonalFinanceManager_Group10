#ifndef REPORTSERVICE_H
#define REPORTSERVICE_H

#include <QList>
#include <QDate>
#include "../models/Report.h"
#include "../models/Transaction.h"

class ReportService
{
public:
    ReportService();
    ~ReportService() = default;

    // Tong thu nhap trong khoang thoi gian [fromDate, toDate]
    double getTotalIncome(const QList<Transaction>& transactions,
                           const QDate& fromDate,
                           const QDate& toDate) const;

    // Tong chi tieu trong khoang thoi gian [fromDate, toDate]
    double getTotalExpense(const QList<Transaction>& transactions,
                            const QDate& fromDate,
                            const QDate& toDate) const;

    // Tao Report tong hop (thu, chi, so du) cho khoang thoi gian
    Report generateReport(const QList<Transaction>& transactions,
                           const QDate& fromDate,
                           const QDate& toDate) const;

    // % thay doi chi tieu so voi ky truoc do (previous range)
    // Neu previous == 0: current == 0 -> tra ve 0.0, current > 0 -> tra ve 100.0
    double compareExpense(const QList<Transaction>& transactions,
                           const QDate& currentFrom, const QDate& currentTo,
                           const QDate& previousFrom, const QDate& previousTo) const;
};

#endif // REPORTSERVICE_H