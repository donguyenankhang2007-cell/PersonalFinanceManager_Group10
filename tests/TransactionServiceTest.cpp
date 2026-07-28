#include <QtTest/QtTest>
#include "../src/core/services/TransactionService.h"
#include "../src/core/database/DatabaseManager.h"

class TransactionServiceTest : public QObject {
    Q_OBJECT

private:
    TransactionService* service;
    DatabaseManager*    dbManager;

private slots:
    void initTestCase() {
        dbManager = new DatabaseManager(":memory:");
        service = new TransactionService(dbManager);
        qDebug() << "=== Bắt đầu chạy test TransactionService ===";
    }

    void cleanupTestCase() {
        delete service;
        delete dbManager;
        qDebug() << "=== Kết thúc test ===";
    }

    void testAddTransaction() {
        Transaction t(0, 50000, "Ăn trưa", "2026-07-24", 1, TransactionType::EXPENSE);
        int id = service->addTransaction(t);
        QVERIFY(id > 0);
        qDebug() << "✅ testAddTransaction PASSED";
    }

    void testGetAllTransactions() {
        auto list = service->getAllTransactions();
        QVERIFY(list.size() >= 1);
        QCOMPARE(list[0].getDescription(), QString("Ăn trưa"));
        qDebug() << "✅ testGetAllTransactions PASSED";
    }

    void testTotalExpense() {
        double total = service->getTotalExpense();
        QCOMPARE(total, 50000.0);
        qDebug() << "✅ testTotalExpense PASSED";
    }

    void testRemoveTransaction() {
        bool ok = service->removeTransaction(1);
        QVERIFY(ok == true);
        auto list = service->getAllTransactions();
        QVERIFY(list.empty());
        qDebug() << "✅ testRemoveTransaction PASSED";
    }
};

QTEST_MAIN(TransactionServiceTest)
#include "TransactionServiceTest.moc"
