#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include "../core/database/DatabaseManager.h"

#include "../core/repositories/BudgetRepository.h"
#include "../core/repositories/RecurringTransactionRepository.h"
#include "../core/repositories/TransactionRepository.h"

#include "../core/services/AccountService.h"
#include "../core/services/CategoryService.h"

#include "../core/services/BudgetService.h"
#include "../core/services/RecurringTransactionService.h"
#include "../core/services/ReportService.h"
#include "../core/services/TransactionService.h"

class AppContext
{
public:
    static AppContext& instance();

    // Trả về DatabaseManager singleton dùng chung cho toàn app
    DatabaseManager& database();

    BudgetRepository& budgetRepository();
    TransactionRepository& transactionRepository();
    RecurringTransactionRepository& recurringTransactionRepository();

    AccountService& accountService();
    CategoryService& categoryService();
    BudgetService& budgetService();
    ReportService& reportService();
    TransactionService& transactionService();
    RecurringTransactionService& recurringTransactionService();

private:
    AppContext();
    ~AppContext();

    AppContext(const AppContext&) = delete;
    AppContext& operator=(const AppContext&) = delete;

private:
    BudgetRepository m_budgetRepository;
    TransactionRepository m_transactionRepository;
    RecurringTransactionRepository m_recurringTransactionRepository;

    AccountService m_accountService;
    CategoryService m_categoryService;
    BudgetService m_budgetService;
    ReportService m_reportService;
    TransactionService m_transactionService;
    RecurringTransactionService m_recurringTransactionService;
};

#endif // APPCONTEXT_H
