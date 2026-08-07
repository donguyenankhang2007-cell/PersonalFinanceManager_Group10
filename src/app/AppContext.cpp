#include "AppContext.h"

AppContext::AppContext()
{
    // RecurringService cần TransactionService để tạo giao dịch thật
    m_recurringTransactionService.setTransactionService(&m_transactionService);
}

AppContext::~AppContext()
{
}

AppContext& AppContext::instance()
{
    static AppContext instance;
    return instance;
}

DatabaseManager& AppContext::database()
{
    return DatabaseManager::instance();
}

BudgetRepository& AppContext::budgetRepository()
{
    return m_budgetRepository;
}

TransactionRepository& AppContext::transactionRepository()
{
    return m_transactionRepository;
}

RecurringTransactionRepository& AppContext::recurringTransactionRepository()
{
    return m_recurringTransactionRepository;
}

AccountService& AppContext::accountService()
{
    return m_accountService;
}

CategoryService& AppContext::categoryService()
{
    return m_categoryService;
}

BudgetService& AppContext::budgetService()
{
    return m_budgetService;
}

ReportService& AppContext::reportService()
{
    return m_reportService;
}

TransactionService& AppContext::transactionService()
{
    return m_transactionService;
}

RecurringTransactionService& AppContext::recurringTransactionService()
{
    return m_recurringTransactionService;
}
