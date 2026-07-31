#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>

#include "pages/DashboardPage.h"
#include "pages/TransactionPage.h"
#include "pages/CategoryPage.h"
#include "pages/BudgetPage.h"
#include "pages/ReportPage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void showDashboard();
    void showTransaction();
    void showCategory();
    void showBudget();
    void showReport();

private:
    Ui::MainWindow *ui;

    QStackedWidget *stackedWidget;

    DashboardPage *dashboard;
    TransactionPage *transaction;
    CategoryPage *category;
    BudgetPage *budget;
    ReportPage *report;

    QPushButton *btnDashboard;
    QPushButton *btnTransaction;
    QPushButton *btnCategory;
    QPushButton *btnBudget;
    QPushButton *btnReport;

    void setupPages();
    void setupConnections();
};

#endif // MAINWINDOW_H
