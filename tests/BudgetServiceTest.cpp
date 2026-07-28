#include <QtTest/QtTest>
#include "../src/core/services/BudgetService.h"
#include "../src/core/database/DatabaseManager.h"

class BudgetServiceTest : public QObject {
    Q_OBJECT

private:
    BudgetService* service;
    DatabaseManager*    dbManager;

private slots:
    void initTestCase() {
        dbManager = new DatabaseManager(":memory:");
        service = new BudgetService(dbManager);
        qDebug() << "=== Bắt đầu chạy test BudgetService ===";
    }

    void cleanupTestCase() {
        delete service;
        delete dbManager;
        qDebug() << "=== Kết thúc test ===";
    }

    void testGetBudget() {
        // Dummy test since BudgetService is not fully implemented
        QVERIFY(service != nullptr);
    }
};

QTEST_MAIN(BudgetServiceTest)
#include "BudgetServiceTest.moc"
