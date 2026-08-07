#include "TransactionService.h"

TransactionService::TransactionService(QObject* parent)
    : QObject(parent)
{
}

bool TransactionService::validate(const Transaction& transaction, QString& errorMessage) const
{
    if (transaction.getAmount() <= 0) {
        errorMessage = "Số tiền phải lớn hơn 0";
        return false;
    }

    QString type = transaction.getType();
    if (type != "income" && type != "expense") {
        errorMessage = "Loại giao dịch phải là 'income' hoặc 'expense'";
        return false;
    }

    if (transaction.getAccountId() <= 0) {
        errorMessage = "Giao dịch phải thuộc về 1 account hợp lệ";
        return false;
    }

    if (transaction.getCategoryId() <= 0) {
        errorMessage = "Giao dịch phải thuộc về 1 category hợp lệ";
        return false;
    }

    errorMessage.clear();
    return true;
}

bool TransactionService::addTransaction(const Transaction& transaction, QString* errorMessage)
{
    QString msg;
    if (!validate(transaction, msg)) {
        if (errorMessage) *errorMessage = msg;
        return false;
    }

    int id = m_transactionRepo.addTransaction(transaction);
    if (id <= 0) {
        if (errorMessage) *errorMessage = "Không thể lưu giao dịch vào cơ sở dữ liệu";
        return false;
    }

    Transaction saved = transaction;
    saved.setId(id);
    applyAccountBalance(saved, true);

    emit transactionAdded(saved);
    return true;
}

bool TransactionService::updateTransaction(const Transaction& transaction, QString* errorMessage)
{
    if (transaction.getId() <= 0) {
        if (errorMessage) *errorMessage = "Giao dịch cần có id hợp lệ";
        return false;
    }

    QString msg;
    if (!validate(transaction, msg)) {
        if (errorMessage) *errorMessage = msg;
        return false;
    }

    Transaction old = m_transactionRepo.getTransactionById(transaction.getId());
    if (old.getId() <= 0) {
        if (errorMessage) *errorMessage = "Không tìm thấy giao dịch cần cập nhật";
        return false;
    }

    if (!m_transactionRepo.updateTransaction(transaction)) {
        if (errorMessage) *errorMessage = "Không thể cập nhật giao dịch";
        return false;
    }

    // Hoàn tác ảnh hưởng của giao dịch cũ, sau đó áp dụng giao dịch mới
    applyAccountBalance(old, false);
    applyAccountBalance(transaction, true);

    emit transactionUpdated(transaction);
    return true;
}

bool TransactionService::removeTransaction(int id, QString* errorMessage)
{
    Transaction old = m_transactionRepo.getTransactionById(id);
    if (old.getId() <= 0) {
        if (errorMessage) *errorMessage = "Không tìm thấy giao dịch cần xóa";
        return false;
    }

    if (!m_transactionRepo.deleteTransaction(id)) {
        if (errorMessage) *errorMessage = "Không thể xóa giao dịch";
        return false;
    }

    applyAccountBalance(old, false);

    emit transactionRemoved(id);
    return true;
}

QVector<Transaction> TransactionService::getAllTransactions()
{
    return m_transactionRepo.getAllTransactions();
}

Transaction TransactionService::getTransactionById(int id)
{
    return m_transactionRepo.getTransactionById(id);
}

double TransactionService::getTotalIncome()
{
    double total = 0.0;
    for (const Transaction& t : m_transactionRepo.getAllTransactions()) {
        if (t.isIncome())
            total += t.getAmount();
    }
    return total;
}

double TransactionService::getTotalExpense()
{
    double total = 0.0;
    for (const Transaction& t : m_transactionRepo.getAllTransactions()) {
        if (t.isExpense())
            total += t.getAmount();
    }
    return total;
}

double TransactionService::getBalance()
{
    return getTotalIncome() - getTotalExpense();
}

int TransactionService::countTransactionsForAccount(int accountId)
{
    int count = 0;
    for (const Transaction& t : m_transactionRepo.getAllTransactions()) {
        if (t.getAccountId() == accountId)
            ++count;
    }
    return count;
}

int TransactionService::countTransactionsForCategory(int categoryId)
{
    int count = 0;
    for (const Transaction& t : m_transactionRepo.getAllTransactions()) {
        if (t.getCategoryId() == categoryId)
            ++count;
    }
    return count;
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

void TransactionService::applyAccountBalance(const Transaction& t, bool add)
{
    Account account = m_accountRepo.getAccountById(t.getAccountId());
    if (account.getId() <= 0)
        return;

    if (t.isIncome()) {
        if (add)
            account.deposit(t.getAmount());
        else
            account.withdraw(t.getAmount());
    } else if (t.isExpense()) {
        if (add)
            account.withdraw(t.getAmount());
        else
            account.deposit(t.getAmount());
    }

    m_accountRepo.updateAccount(account);
}
