// ============================================
// File: tests/BudgetServiceTest.cpp
// Mo ta: Unit test cho BudgetService
//        (CRUD + tinh toan ngân sách)
// ============================================
#include <QtTest/QtTest>
#include <QTemporaryDir>
#include <QSqlQuery>

#include "../src/core/services/BudgetService.h"
#include "../src/core/services/TransactionService.h"
#include "../src/core/repositories/AccountRepository.h"
#include "../src/core/repositories/CategoryRepository.h"
#include "../src/core/database/DatabaseManager.h"

class BudgetServiceTest : public QObject
{
    Q_OBJECT

private:
    QTemporaryDir m_tempDir;
    BudgetService *budgetService;
    TransactionService *txService;
    AccountRepository m_accountRepo;
    CategoryRepository m_categoryRepo;

    int addAccount(const QString &name, double balance)
    {
        Account acc(0, name, balance);
        m_accountRepo.addAccount(acc);
        QVector<Account> all = m_accountRepo.getAllAccounts();
        for (const Account &a : all) {
            if (a.getName() == name)
                return a.getId();
        }
        return -1;
    }

    int addExpenseCategory()
    {
        Category cat(0, "Ăn uống", CategoryType::Expense, "#ff5733", "");
        m_categoryRepo.addCategory(cat);
        QVector<Category> all = m_categoryRepo.getAllCategories();
        for (const Category &c : all) {
            if (c.getName() == "Ăn uống")
                return c.getId();
        }
        return -1;
    }

    // Xóa toàn bộ dữ liệu để mỗi test độc lập với nhau
    void clearDatabase()
    {
        QSqlQuery query;
        query.exec("DELETE FROM Transactions");
        query.exec("DELETE FROM Budget");
        query.exec("DELETE FROM Account");
        query.exec("DELETE FROM Category");
    }

private slots:
    void initTestCase()
    {
        QVERIFY(m_tempDir.isValid());
        QVERIFY(DatabaseManager::instance().openDatabase(
            m_tempDir.filePath("test.db")));
        DatabaseManager::instance().initializeDatabase();
    }

    void init()
    {
        clearDatabase();
        budgetService = new BudgetService();
        txService = new TransactionService();
    }

    void cleanup()
    {
        delete budgetService;
        delete txService;
    }

    void cleanupTestCase()
    {
        DatabaseManager::instance().closeDatabase();
    }

    void testAddBudget()
    {
        int categoryId = addExpenseCategory();
        QVERIFY(categoryId > 0);

        Budget budget(0, categoryId, 2000000, 7, 2026);
        QString errorMessage;

        QVERIFY(budgetService->addBudget(budget, &errorMessage));
        QCOMPARE(budgetService->getAllBudgets().size(), 1);
    }

    void testCalculateSpent()
    {
        int accountId = addAccount("Cash", 0);
        int categoryId = addExpenseCategory();
        QVERIFY(accountId > 0 && categoryId > 0);

        // Chi 500000 trong tháng 7/2026
        Transaction t1(0, accountId, categoryId, 500000,
                       QDate(2026, 7, 10), "Ăn trưa", "expense");
        // Thu nhập không tính vào chi tiêu
        Transaction t2(0, accountId, categoryId, 300000,
                       QDate(2026, 7, 11), "Lương", "income");
        // Chi vào tháng khác không tính
        Transaction t3(0, accountId, categoryId, 400000,
                       QDate(2026, 6, 10), "Chi tháng 6", "expense");

        QVERIFY(txService->addTransaction(t1));
        QVERIFY(txService->addTransaction(t2));
        QVERIFY(txService->addTransaction(t3));

        QVector<Transaction> all = txService->getAllTransactions();
        QList<Transaction> txList(all.begin(), all.end());

        Budget budget(0, categoryId, 2000000, 7, 2026);

        QCOMPARE(budgetService->getSpentAmount(budget, txList), 500000.0);
        QCOMPARE(budgetService->getUsagePercent(budget, txList), 0.25);
        QVERIFY(!budgetService->isOverBudget(budget, txList));
    }

    void testIsOverBudget()
    {
        int accountId = addAccount("Cash", 0);
        int categoryId = addExpenseCategory();
        QVERIFY(accountId > 0 && categoryId > 0);

        Transaction t1(0, accountId, categoryId, 1500000,
                       QDate(2026, 7, 10), "Chi", "expense");
        Transaction t2(0, accountId, categoryId, 1000000,
                       QDate(2026, 7, 11), "Chi", "expense");

        QVERIFY(txService->addTransaction(t1));
        QVERIFY(txService->addTransaction(t2));

        QVector<Transaction> all = txService->getAllTransactions();
        QList<Transaction> txList(all.begin(), all.end());

        Budget budget(0, categoryId, 2000000, 7, 2026);

        // Đã chi 2.500.000 > hạn mức 2.000.000
        QVERIFY(budgetService->isOverBudget(budget, txList));
        QVERIFY(budgetService->getUsagePercent(budget, txList) > 1.0);
    }

    void testRemoveBudget()
    {
        int categoryId = addExpenseCategory();
        QVERIFY(categoryId > 0);

        Budget budget(0, categoryId, 1000000, 7, 2026);
        QVERIFY(budgetService->addBudget(budget));

        int id = budgetService->getAllBudgets().first().getId();

        QVERIFY(budgetService->removeBudget(id));
        QVERIFY(budgetService->getAllBudgets().isEmpty());
    }
};

QTEST_MAIN(BudgetServiceTest)
#include "BudgetServiceTest.moc"
