#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QPushButton>

#include "pages/AccountPage.h"
#include "pages/DashboardPage.h"
#include "pages/CategoryPage.h"
#include "pages/TransactionPage.h"
#include "pages/BudgetPage.h"
#include "pages/RecurringPage.h"
#include "pages/SettingsPage.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

    // Chỉ mục các trang trong QStackedWidget
    enum PageIndex {
        DashboardIndex = 0,
        AccountIndex   = 1,
        CategoryIndex  = 2,
        TransactionIndex = 3,
        BudgetIndex    = 4,
        RecurringIndex = 5,
        SettingsIndex  = 6
    };

private slots:
    void showPage(int index);

private:
    void setupPages();
    void setupConnections();

    void buildNavButton(QPushButton *btn, const QString &icon,
                        const QString &tooltip);
    void syncNav(int activeIndex);

    QStackedWidget *stackedWidget;

    DashboardPage *dashboard;
    AccountPage   *accountPage;
    CategoryPage  *category;
    TransactionPage *transaction;
    BudgetPage    *budget;
    RecurringPage *recurringPage;
    SettingsPage  *settingsPage;

    QPushButton *btnDashboard;
    QPushButton *btnCategory;
    QPushButton *btnTransaction;
    QPushButton *btnBudget;
    QPushButton *btnRecurring;
    QPushButton *btnSettings;
    QPushButton *avatarButton;

    class QLabel *hoverTooltip;
};

#endif // MAINWINDOW_H
