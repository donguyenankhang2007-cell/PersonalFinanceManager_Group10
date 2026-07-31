#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>
#include <QLabel>

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
    // Xu ly khi nguoi dung bam nut navigation
    void onNavButtonClicked(int index);

private:
    void setupUI();
    void createSidebar(QWidget *sidebarWidget);
    void createPages();
    void setActivePage(int index);
    QString getGlobalStylesheet() const;

    // Navigation
    QStackedWidget *m_stackedWidget;
    QList<QPushButton*> m_navButtons;
    int m_currentIndex;

    // Cac trang chinh
    DashboardPage   *m_dashboardPage;
    TransactionPage *m_transactionPage;
    CategoryPage    *m_categoryPage;
    BudgetPage      *m_budgetPage;
    ReportPage      *m_reportPage;
};

#endif // MAINWINDOW_H
