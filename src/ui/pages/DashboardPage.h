#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>

class DashboardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardPage(QWidget *parent = nullptr);

public slots:
    // Lam moi du lieu tong quan
    void refreshData();

private:
    void setupUI();
    QWidget* createSummaryCard(const QString &title, QLabel *valueLabel,
                                const QString &accentColor);

    // Labels hien thi so lieu tong quan
    QLabel *m_totalIncomeValue;
    QLabel *m_totalExpenseValue;
    QLabel *m_balanceValue;
    QLabel *m_transactionCountValue;

    // Bang giao dich gan day
    QTableWidget *m_recentTable;
};

#endif // DASHBOARDPAGE_H
