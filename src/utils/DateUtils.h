#ifndef DATEUTILS_H
#define DATEUTILS_H

#include <QDate>
#include <QString>

// ============================================
// File: src/utils/DateUtils.h
// Nguoi viet: Minh Hao
// Mo ta: Cac ham tien ich xu ly ngay thang,
//         phuc vu cho ReportService va BudgetService
// ============================================

class DateUtils
{
public:
    // Lay ngay dau tien cua thang (VD: thang 7/2026 -> 01/07/2026)
    static QDate firstDayOfMonth(int year, int month)
    {
        return QDate(year, month, 1);
    }

    // Lay ngay cuoi cung cua thang (VD: thang 7/2026 -> 31/07/2026)
    static QDate lastDayOfMonth(int year, int month)
    {
        // daysInMonth() cua Qt tu tinh so ngay chinh xac (28/29/30/31)
        return QDate(year, month, QDate(year, month, 1).daysInMonth());
    }

    // Kiem tra mot ngay co nam trong khoang [fromDate, toDate] khong
    static bool isInRange(const QDate& date,
                          const QDate& fromDate,
                          const QDate& toDate)
    {
        return date >= fromDate && date <= toDate;
    }

    // Dinh dang QDate thanh chuoi "dd/MM/yyyy" (hien thi cho nguoi dung)
    static QString formatDisplay(const QDate& date)
    {
        return date.toString("dd/MM/yyyy");
    }

    // Dinh dang QDate thanh chuoi "yyyy-MM-dd" (luu vao database)
    static QString formatDB(const QDate& date)
    {
        return date.toString("yyyy-MM-dd");
    }

    // Parse chuoi "yyyy-MM-dd" tu DB thanh QDate
    static QDate parseDB(const QString& dateStr)
    {
        return QDate::fromString(dateStr, "yyyy-MM-dd");
    }

    // Lay thang truoc do (xu ly truong hop thang 1 -> quay ve thang 12 nam truoc)
    // Dung de compareExpense() trong ReportService tinh ky truoc
    static void previousMonth(int year, int month, int& outYear, int& outMonth)
    {
        if (month == 1) {
            outYear  = year - 1;
            outMonth = 12;
        } else {
            outYear  = year;
            outMonth = month - 1;
        }
    }

    // Kiem tra nam nhuan
    static bool isLeapYear(int year)
    {
        return QDate::isLeapYear(year);
    }
};

#endif // DATEUTILS_H
