#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>

#include "pages/AccountPage.h"
#include "pages/DashboardPage.h"
#include "pages/TransactionPage.h"
#include "pages/CategoryPage.h"
#include "pages/BudgetPage.h"
#include "pages/ReportPage.h"

// Forward declarations — cac trang duoc tao trong .cpp
class DashboardPage;
class TransactionPage;
class CategoryPage;
class BudgetPage;
class ReportPage;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void showDashboard();
    void showAccounts();
    void showTransaction();
    void showCategory();
    void showBudget();
    void showReport();

private:
    Ui::MainWindow *ui;

    QStackedWidget *stackedWidget;

    DashboardPage *dashboard;
    AccountPage *accountPage;
    TransactionPage *transaction;
    CategoryPage *category;
    BudgetPage *budget;
    ReportPage *report;

    QPushButton *btnDashboard;
    QPushButton *btnAccounts;
    QPushButton *btnTransaction;
    QPushButton *btnCategory;
    QPushButton *btnBudget;
    QPushButton *btnReport;

    void setupPages();
    void setupConnections();
};

#endif // MAINWINDOW_H
