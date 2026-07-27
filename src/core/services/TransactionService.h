#ifndef TRANSACTIONSERVICE_H
#define TRANSACTIONSERVICE_H

#include <QList>
#include <QString>
#include "../models/Transaction.h"

class TransactionService
{
public:
    TransactionService();
    ~TransactionService() = default;

    // Kiem tra 1 transaction co hop le khong (amount > 0, type la "income" hoac "expense")
    // Neu khong hop le, tra ve false va gan ly do vao errorMessage (out param)
    bool validate(const Transaction& transaction, QString& errorMessage) const;

    // Loc danh sach transaction theo accountId
    QList<Transaction> filterByAccount(const QList<Transaction>& transactions,
                                        int accountId) const;

    // Loc danh sach transaction theo categoryId
    QList<Transaction> filterByCategory(const QList<Transaction>& transactions,
                                         int categoryId) const;

    // Loc danh sach transaction trong khoang thoi gian
    QList<Transaction> filterByDateRange(const QList<Transaction>& transactions,
                                          const QDate& fromDate,
                                          const QDate& toDate) const;

    // Tong so tien cua 1 danh sach transaction (co the dung sau khi filter)
    double sumAmount(const QList<Transaction>& transactions) const;
};

#endif // TRANSACTIONSERVICE_H