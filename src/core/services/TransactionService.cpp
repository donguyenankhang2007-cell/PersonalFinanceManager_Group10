#include "TransactionService.h"

TransactionService::TransactionService()
{
}

bool TransactionService::validate(const Transaction& transaction, QString& errorMessage) const
{
    if (transaction.getAmount() <= 0) {
        errorMessage = "So tien phai lon hon 0";
        return false;
    }

    QString type = transaction.getType();
    if (type != "income" && type != "expense") {
        errorMessage = "Loai giao dich phai la 'income' hoac 'expense'";
        return false;
    }

    if (transaction.getAccountId() <= 0) {
        errorMessage = "Giao dich phai thuoc ve 1 account hop le";
        return false;
    }

    if (transaction.getCategoryId() <= 0) {
        errorMessage = "Giao dich phai thuoc ve 1 category hop le";
        return false;
    }

    errorMessage.clear();
    return true;
}

QList<Transaction> TransactionService::filterByAccount(const QList<Transaction>& transactions,
                                                         int accountId) const
{
    QList<Transaction> result;
    for (const Transaction& t : transactions) {
        if (t.getAccountId() == accountId) {
            result.append(t);
        }
    }
    return result;
}

QList<Transaction> TransactionService::filterByCategory(const QList<Transaction>& transactions,
                                                          int categoryId) const
{
    QList<Transaction> result;
    for (const Transaction& t : transactions) {
        if (t.getCategoryId() == categoryId) {
            result.append(t);
        }
    }
    return result;
}

QList<Transaction> TransactionService::filterByDateRange(const QList<Transaction>& transactions,
                                                           const QDate& fromDate,
                                                           const QDate& toDate) const
{
    QList<Transaction> result;
    for (const Transaction& t : transactions) {
        if (t.getDate() >= fromDate && t.getDate() <= toDate) {
            result.append(t);
        }
    }
    return result;
}

double TransactionService::sumAmount(const QList<Transaction>& transactions) const
{
    double total = 0.0;
    for (const Transaction& t : transactions) {
        total += t.getAmount();
    }
    return total;
}