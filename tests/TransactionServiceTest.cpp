// ============================================
// File: tests/TransactionServiceTest.cpp
// Mo ta: Unit test cho TransactionService
//        (CRUD + signal + cap nhat so du account)
// ============================================
#include <QtTest/QtTest>
#include <QTemporaryDir>
#include <QSqlQuery>

#include "../src/core/services/TransactionService.h"
#include "../src/core/repositories/AccountRepository.h"
#include "../src/core/repositories/CategoryRepository.h"
#include "../src/core/database/DatabaseManager.h"

class TransactionServiceTest : public QObject
{
    Q_OBJECT

private:
    QTemporaryDir m_tempDir;
    TransactionService *service;
    AccountRepository m_accountRepo;
    int m_categoryId;

    int addAccount(const QString &name, double balance)
    {
        Account acc(0, name, balance);
        m_accountRepo.addAccount(acc);
        // Lấy id vừa thêm
        QVector<Account> all = m_accountRepo.getAllAccounts();
        for (const Account &a : all) {
            if (a.getName() == name)
                return a.getId();
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

        // Tạo lại 1 category hợp lệ để thỏa ràng buộc khóa ngoại
        CategoryRepository categoryRepo;
        Category cat(0, "Test", CategoryType::Expense, "#000000", "");
        categoryRepo.addCategory(cat);
        QVector<Category> all = categoryRepo.getAllCategories();
        QVERIFY(!all.isEmpty());
        m_categoryId = all.first().getId();
        QVERIFY(m_categoryId > 0);

        service = new TransactionService();
    }

    void cleanup()
    {
        delete service;
    }

    void cleanupTestCase()
    {
        DatabaseManager::instance().closeDatabase();
    }

    void testAddTransaction()
    {
        int accountId = addAccount("Cash", 0);
        QVERIFY(accountId > 0);

        Transaction tx(0, accountId, m_categoryId, 100000,
                       QDate(2026, 7, 10), "Lương", "income");

        QString errorMessage;
        QVERIFY(service->addTransaction(tx, &errorMessage));

        QCOMPARE(service->getAllTransactions().size(), 1);

        // Số dư account phải tăng đúng 100000
        Account acc = m_accountRepo.getAccountById(accountId);
        QCOMPARE(acc.getBalance(), 100000.0);
    }

    void testValidate()
    {
        int accountId = addAccount("Bank", 0);
        QVERIFY(accountId > 0);

        QString errorMessage;

        // Số tiền <= 0 → không hợp lệ
        Transaction badAmount(0, accountId, m_categoryId, -5000,
                              QDate(2026, 7, 10), "x", "expense");
        QVERIFY(!service->addTransaction(badAmount, &errorMessage));
        QVERIFY(!errorMessage.isEmpty());

        // Loại giao dịch sai → không hợp lệ
        Transaction badType(0, accountId, m_categoryId, 5000,
                            QDate(2026, 7, 10), "x", "transfer");
        QVERIFY(!service->addTransaction(badType, &errorMessage));

        // accountId không hợp lệ → không hợp lệ
        Transaction badAccount(0, -1, m_categoryId, 5000,
                               QDate(2026, 7, 10), "x", "expense");
        QVERIFY(!service->addTransaction(badAccount, &errorMessage));

        // Không có giao dịch nào được thêm
        QCOMPARE(service->getAllTransactions().size(), 0);
    }

    void testTotals()
    {
        int accountId = addAccount("Wallet", 0);
        QVERIFY(accountId > 0);

        Transaction income(0, accountId, m_categoryId, 50000,
                           QDate(2026, 7, 1), "Thu", "income");
        Transaction expense(0, accountId, m_categoryId, 20000,
                            QDate(2026, 7, 2), "Chi", "expense");

        QVERIFY(service->addTransaction(income));
        QVERIFY(service->addTransaction(expense));

        QCOMPARE(service->getTotalIncome(), 50000.0);
        QCOMPARE(service->getTotalExpense(), 20000.0);
        QCOMPARE(service->getBalance(), 30000.0);
    }

    void testUpdateTransaction()
    {
        int accountId = addAccount("Visa", 0);
        QVERIFY(accountId > 0);

        Transaction tx(0, accountId, m_categoryId, 30000,
                       QDate(2026, 7, 5), "Cũ", "expense");
        QVERIFY(service->addTransaction(tx));
        QVERIFY(service->getAllTransactions().size() == 1);

        int id = service->getAllTransactions().first().getId();
        QVERIFY(id > 0);

        // Đổi thành income + số tiền khác
        Transaction updated(id, accountId, m_categoryId, 50000,
                            QDate(2026, 7, 6), "Mới", "income");
        QVERIFY(service->updateTransaction(updated));

        Account acc = m_accountRepo.getAccountById(accountId);
        QCOMPARE(acc.getBalance(), 50000.0);
    }

    void testRemoveTransaction()
    {
        int accountId = addAccount("Momo", 100000);
        QVERIFY(accountId > 0);

        Transaction tx(0, accountId, m_categoryId, 40000,
                       QDate(2026, 7, 8), "Mua sắm", "expense");
        QVERIFY(service->addTransaction(tx));

        int id = service->getAllTransactions().first().getId();

        QVERIFY(service->removeTransaction(id));
        QVERIFY(service->getAllTransactions().isEmpty());

        // Xóa giao dịch → số dư account khôi phục về ban đầu
        Account acc = m_accountRepo.getAccountById(accountId);
        QCOMPARE(acc.getBalance(), 100000.0);
    }

    void testSignalEmitted()
    {
        int accountId = addAccount("Signal", 0);
        QVERIFY(accountId > 0);

        QSignalSpy spy(service, &TransactionService::transactionAdded);

        Transaction tx(0, accountId, m_categoryId, 10000,
                       QDate(2026, 7, 9), "Test", "income");
        QVERIFY(service->addTransaction(tx));

        QCOMPARE(spy.count(), 1);
    }
};

QTEST_MAIN(TransactionServiceTest)
#include "TransactionServiceTest.moc"
