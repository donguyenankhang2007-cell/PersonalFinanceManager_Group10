#include "RecurringTransactionService.h"
#include "TransactionService.h"
#include "../models/Transaction.h"

#include <QDate>
#include <QDebug>

RecurringTransactionService::RecurringTransactionService(QObject* parent)
    : QObject(parent)
    , m_transactionService(nullptr)
{
}

void RecurringTransactionService::setTransactionService(TransactionService* service)
{
    m_transactionService = service;
}

QDate RecurringTransactionService::advanceDate(const QDate& date,
                                               const QString& frequency)
{
    if (frequency == "daily")
        return date.addDays(1);
    if (frequency == "weekly")
        return date.addDays(7);
    if (frequency == "yearly")
        return date.addYears(1);
    return date.addMonths(1); // mặc định: monthly
}

bool RecurringTransactionService::validate(const RecurringTransaction& recurring,
                                           QString& errorMessage) const
{
    if (recurring.getAmount() <= 0) {
        errorMessage = "Amount must be greater than 0!";
        return false;
    }
    if (recurring.getAccountId() <= 0) {
        errorMessage = "Please choose an account!";
        return false;
    }
    if (recurring.getCategoryId() <= 0) {
        errorMessage = "Please choose a category!";
        return false;
    }
    if (!recurring.getNextDate().isValid()) {
        errorMessage = "Next date is not valid!";
        return false;
    }
    if (recurring.hasEndDate() && recurring.getEndDate() < recurring.getNextDate()) {
        errorMessage = "End date must not be before the next date!";
        return false;
    }
    return true;
}

bool RecurringTransactionService::add(const RecurringTransaction& recurring,
                                      QString* errorMessage)
{
    QString err;
    if (!validate(recurring, err)) {
        if (errorMessage) *errorMessage = err;
        return false;
    }

    int id = m_repo.add(recurring);
    if (id <= 0) {
        if (errorMessage) *errorMessage = "Failed to add recurring transaction!";
        return false;
    }

    RecurringTransaction saved = recurring;
    saved.setId(id);
    emit recurringAdded(saved);
    return true;
}

bool RecurringTransactionService::update(const RecurringTransaction& recurring,
                                         QString* errorMessage)
{
    QString err;
    if (!validate(recurring, err)) {
        if (errorMessage) *errorMessage = err;
        return false;
    }

    if (!m_repo.update(recurring)) {
        if (errorMessage) *errorMessage = "Failed to update recurring transaction!";
        return false;
    }

    emit recurringUpdated(recurring);
    return true;
}

bool RecurringTransactionService::remove(int id, QString* errorMessage)
{
    if (!m_repo.remove(id)) {
        if (errorMessage) *errorMessage = "Failed to remove recurring transaction!";
        return false;
    }
    emit recurringRemoved(id);
    return true;
}

QVector<RecurringTransaction> RecurringTransactionService::getAll()
{
    return m_repo.getAll();
}

int RecurringTransactionService::generateDueTransactions()
{
    QDate today = QDate::currentDate();
    QVector<RecurringTransaction> dueList = m_repo.getActive();

    int generated = 0;
    for (const RecurringTransaction& item : dueList) {
        if (item.getNextDate() > today)
            continue;

        RecurringTransaction current = item;

        // Catch-up: sinh tất cả kỳ đến hạn cho tới hôm nay
        while (current.getNextDate() <= today) {
            if (current.hasEndDate() && current.getNextDate() > current.getEndDate())
                break;

            // Tạo giao dịch thật (qua TransactionService để cập nhật số dư + signal)
            Transaction tx(0,
                           current.getAccountId(),
                           current.getCategoryId(),
                           current.getAmount(),
                           current.getNextDate(),
                           current.getNote(),
                           current.getType());
            if (m_transactionService) {
                QString err;
                m_transactionService->addTransaction(tx, &err);
            }
            ++generated;

            QDate next = advanceDate(current.getNextDate(), current.getFrequency());

            // Điều chỉnh nếu kỳ tiếp theo vượt endDate
            if (current.hasEndDate() && next > current.getEndDate()) {
                next = current.getEndDate().addDays(1);
            }
            current.setNextDate(next);
        }

        // Nếu đã hết hạn → tắt active
        if (current.hasEndDate() && current.getNextDate() > current.getEndDate()) {
            current.setActive(false);
        }

        m_repo.update(current);
    }

    return generated;
}
