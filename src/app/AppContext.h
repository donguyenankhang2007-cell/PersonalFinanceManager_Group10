#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include "../core/database/DatabaseManager.h"

#include "../core/repositories/AccountRepository.h"
#include "../core/repositories/CategoryRepository.h"
#include "../core/repositories/TransactionRepository.h"

#include "../core/services/BudgetService.h"
#include "../core/services/ReportService.h"
#include "../core/services/TransactionService.h"

class AppContext
{
public:
    static AppContext& instance();

    DatabaseManager& database();

    AccountRepository& accountRepository();
    CategoryRepository& categoryRepository();
    TransactionRepository& transactionRepository();

    BudgetService& budgetService();
    ReportService& reportService();
    TransactionService& transactionService();

private:
    AppContext();
    ~AppContext();

    AppContext(const AppContext&) = delete;
    AppContext& operator=(const AppContext&) = delete;

private:
    DatabaseManager m_database;

    AccountRepository m_accountRepository;
    CategoryRepository m_categoryRepository;
    TransactionRepository m_transactionRepository;

    BudgetService m_budgetService;
    ReportService m_reportService;
    TransactionService m_transactionService;
};

#endif // APPCONTEXT_H
