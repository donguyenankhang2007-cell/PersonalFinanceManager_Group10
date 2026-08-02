#ifndef MONEYUTILS_H
#define MONEYUTILS_H

#include <QString>
#include <QLocale>
#include <cmath>

// ============================================
// File: src/utils/MoneyUtils.h
// Nguoi viet: Minh Hao
// Mo ta: Cac ham tien ich dinh dang so tien VND,
//         phuc vu hien thi cho GUI (Viet Tuong)
//         và ReportService
// ============================================

class MoneyUtils
{
public:
    // Định dạng số tiền VND có dấu phân cách hàng nghìn
    // VD: 1500000 -> "1.500.000 VND"
    //     -50000  -> "-50.000 VND"
    static QString formatVND(double amount)
    {
        // QLocale("vi_VN") su dung dau cham phan cach hang nghin
        QLocale locale(QLocale::Vietnamese, QLocale::Vietnam);
        // toString() tu dong them dau phan cach, 'f' = fixed, 0 = 0 chu so thap phan
        QString formatted = locale.toString(amount, 'f', 0);
        return formatted + " VND";
    }

    // Dinh dang gon (compact) cho Dashboard / tong quan
    // VD: 1500000  -> "1.5M"
    //     750000   -> "750K"
    //     50000    -> "50K"
    static QString formatCompact(double amount)
    {
        double abs = std::abs(amount);
        QString sign = (amount < 0) ? "-" : "";

        if (abs >= 1000000000.0) {
            return sign + QString::number(abs / 1000000000.0, 'f', 1) + " tỷ";
        } else if (abs >= 1000000.0) {
            return sign + QString::number(abs / 1000000.0, 'f', 1) + "tr";
        } else if (abs >= 1000.0) {
            return sign + QString::number(abs / 1000.0, 'f', 0) + "K";
        } else {
            return sign + QString::number(abs, 'f', 0);
        }
    }

    // Kiem tra so tien co hop le khong (phai lon hon 0)
    // Dung ket hop voi TransactionService::validate()
    static bool isValidAmount(double amount)
    {
        return amount > 0.0;
    }

    // Lam tron xuong den 1000 VND (ap dung khi can lam tron ngan sach)
    // VD: 1234567 -> 1234000
    static double roundDownToThousand(double amount)
    {
        return std::floor(amount / 1000.0) * 1000.0;
    }

    // Tinh so du = thu nhap - chi tieu
    // Trả về âm nếu chi vượt quá thu
    static double calcBalance(double totalIncome, double totalExpense)
    {
        return totalIncome - totalExpense;
    }
};

#endif // MONEYUTILS_H
