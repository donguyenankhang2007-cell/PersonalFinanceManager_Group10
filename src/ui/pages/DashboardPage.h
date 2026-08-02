#ifndef DASHBOARDPAGE_H
#define DASHBOARDPAGE_H

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include "core/models/Transaction.h"

QT_BEGIN_NAMESPACE
class QChartView;
class QWidget;
class QTableWidget;
class QVBoxLayout;
class QGraphicsTextItem;
class QPieSlice;
QT_END_NAMESPACE

class DashboardPage : public QWidget
{
    Q_OBJECT

public:
    explicit DashboardPage(QWidget *parent = nullptr);

public slots:
    void loadData();

private slots:
    void setThisMonth();
    void setLastMonth();
    void exportCsv();
    void onDonutHovered(QPieSlice *slice, bool state);

private:
    void buildLayout();
    void updateCharts();
    void updateTrendChart();
    void setChartTheme();
    void makeCard(QWidget *&card, QLabel *&valueLabel,
                  const QString &title, const QString &valueObjectName);
    void makeCompactCard(QWidget *&card, QLabel *&valueLabel,
                         const QString &title);
    void updateTopCategories(const QList<Transaction>& txList);
    void updateRecentTransactions(const QList<Transaction>& txList);

    QLabel *accountCountLabel;
    QLabel *categoryCountLabel;
    QLabel *transactionCountLabel;
    QLabel *totalIncomeLabel;
    QLabel *totalExpenseLabel;
    QLabel *balanceLabel;
    
    QLabel *compareIncomeLabel;
    QLabel *compareExpenseLabel;
    QLabel *compareBalanceLabel;
    
    QComboBox *monthCb;
    QComboBox *yearCb;

    QLabel *monthIncomeLabel;
    QLabel *monthExpenseLabel;
    QLabel *monthNetLabel;

    QWidget *chartsCard;
    QChartView *donutView;
    QWidget *donutLegendWidget;
    QVBoxLayout *donutLegendLayout;
    QGraphicsTextItem *donutCenterText;
    QChartView *barView; // This will now be the trend line chart


    QTableWidget *topCategoriesTable;
    QTableWidget *recentTable;
};

#endif // DASHBOARDPAGE_H
