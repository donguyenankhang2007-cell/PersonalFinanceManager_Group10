#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QLabel>

class DashboardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardPage(QWidget *parent = nullptr);

public slots:
    void loadData();

private:
    QLabel *accountCountLabel;
    QLabel *categoryCountLabel;
    QLabel *transactionCountLabel;
    QLabel *totalIncomeLabel;
    QLabel *totalExpenseLabel;
    QLabel *balanceLabel;
};

#endif
