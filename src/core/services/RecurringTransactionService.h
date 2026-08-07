#ifndef RECURRINGTRANSACTIONSERVICE_H
#define RECURRINGTRANSACTIONSERVICE_H

#include <QObject>
#include <QVector>
#include "../models/RecurringTransaction.h"
#include "../repositories/RecurringTransactionRepository.h"

class TransactionService;

class RecurringTransactionService : public QObject
{
    Q_OBJECT

public:
    explicit RecurringTransactionService(QObject* parent = nullptr);

    // === CRUD ===
    bool add(const RecurringTransaction& recurring,
             QString* errorMessage = nullptr);
    bool update(const RecurringTransaction& recurring,
                QString* errorMessage = nullptr);
    bool remove(int id, QString* errorMessage = nullptr);

    QVector<RecurringTransaction> getAll();

    // === Sinh giao dịch đến hạn ===
    // Duyệt các giao dịch định kỳ còn active có nextDate <= today,
    // tự tạo giao dịch thật qua TransactionService và cập nhật nextDate.
    // Trả về số giao dịch đã sinh.
    int generateDueTransactions();

    // === Kiểm tra tính hợp lệ (thuần túy, không cần DB) ===
    bool validate(const RecurringTransaction& recurring,
                  QString& errorMessage) const;

    // Gắn TransactionService để sinh giao dịch thật (gọi khi khởi tạo AppContext)
    void setTransactionService(TransactionService* service);

    // Cộng nextDate theo frequency
    static QDate advanceDate(const QDate& date, const QString& frequency);

signals:
    void recurringAdded(const RecurringTransaction& recurring);
    void recurringUpdated(const RecurringTransaction& recurring);
    void recurringRemoved(int recurringId);

private:
    RecurringTransactionRepository m_repo;
    TransactionService* m_transactionService;
};

#endif // RECURRINGTRANSACTIONSERVICE_H
