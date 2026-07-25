#include "AppContext.h"

AppContext::AppContext()
{
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
    return m_database;
}

AccountRepository& AppContext::accountRepository()
{
    return m_accountRepository;
}

CategoryRepository& AppContext::categoryRepository()
{
    return m_categoryRepository;
}

TransactionRepository& AppContext::transactionRepository()
{
    return m_transactionRepository;
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
