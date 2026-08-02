#ifndef TRANSACTIONSERVICE_H
#define TRANSACTIONSERVICE_H

#include <QObject>
#include <QList>
#include <QString>
#include <QDate>
#include "../models/Transaction.h"
#include "../repositories/TransactionRepository.h"
#include "../repositories/AccountRepository.h"

class TransactionService : public QObject
{
    Q_OBJECT

public:
    explicit TransactionService(QObject* parent = nullptr);

    // === CRUD — tự cập nhật số dư account và phát signal (Observer) ===
    // Trả về false + gán lý do vào errorMessage (out param) nếu không hợp lệ
    bool addTransaction(const Transaction& transaction, QString* errorMessage = nullptr);
    bool updateTransaction(const Transaction& transaction, QString* errorMessage = nullptr);
    bool removeTransaction(int id, QString* errorMessage = nullptr);

    QVector<Transaction> getAllTransactions();
    Transaction getTransactionById(int id);

    // === Thống kê ===
    double getTotalIncome();
    double getTotalExpense();
    double getBalance();

    // Đếm giao dịch tham chiếu tới 1 account/category (dùng khi xóa để bảo vệ dữ liệu)
    int countTransactionsForAccount(int accountId);
    int countTransactionsForCategory(int categoryId);

    // === Hàm thuần túy (không cần DB) ===
    bool validate(const Transaction& transaction, QString& errorMessage) const;

    QList<Transaction> filterByAccount(const QList<Transaction>& transactions,
                                       int accountId) const;

    QList<Transaction> filterByCategory(const QList<Transaction>& transactions,
                                        int categoryId) const;

    QList<Transaction> filterByDateRange(const QList<Transaction>& transactions,
                                         const QDate& fromDate,
                                         const QDate& toDate) const;

    double sumAmount(const QList<Transaction>& transactions) const;

signals:
    void transactionAdded(const Transaction& transaction);
    void transactionUpdated(const Transaction& transaction);
    void transactionRemoved(int transactionId);

private:
    // Cộng/trừ số dư account theo 1 giao dịch (add=true → áp dụng, add=false → hoàn tác)
    void applyAccountBalance(const Transaction& t, bool add);

    TransactionRepository m_transactionRepo;
    AccountRepository m_accountRepo;
};

#endif // TRANSACTIONSERVICE_H
