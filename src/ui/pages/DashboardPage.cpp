#include "DashboardPage.h"
#include "../../app/AppContext.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(20);

    // Tiêu đề
    QLabel *title = new QLabel("Dashboard");
    title->setStyleSheet(
        "font-size: 22px;"
        "font-weight: bold;"
        "color: #1e272e;"
        "padding-bottom: 5px;");

    // Style chung cho card
    QString cardStyle =
        "font-size: 16px;"
        "color: #2f3640;"
        "background-color: #ffffff;"
        "padding: 20px;"
        "border-radius: 10px;"
        "border: 1px solid #dcdde1;";

    QString valueStyle =
        "font-size: 24px;"
        "font-weight: bold;"
        "color: #1e272e;"
        "background-color: transparent;"
        "border: none;"
        "padding: 0px;";

    QString subtitleStyle =
        "font-size: 12px;"
        "color: #808e9b;"
        "background-color: transparent;"
        "border: none;"
        "padding: 0px;";

    // === Grid layout cho cards ===
    QGridLayout *grid = new QGridLayout();
    grid->setSpacing(15);

    // Card 1 - Accounts
    QWidget *card1 = new QWidget();
    card1->setStyleSheet(cardStyle);
    QVBoxLayout *c1Layout = new QVBoxLayout(card1);
    QLabel *c1Title = new QLabel("Accounts");
    c1Title->setStyleSheet(subtitleStyle);
    accountCountLabel = new QLabel("0");
    accountCountLabel->setStyleSheet(valueStyle);
    c1Layout->addWidget(c1Title);
    c1Layout->addWidget(accountCountLabel);

    // Card 2 - Categories
    QWidget *card2 = new QWidget();
    card2->setStyleSheet(cardStyle);
    QVBoxLayout *c2Layout = new QVBoxLayout(card2);
    QLabel *c2Title = new QLabel("Categories");
    c2Title->setStyleSheet(subtitleStyle);
    categoryCountLabel = new QLabel("0");
    categoryCountLabel->setStyleSheet(valueStyle);
    c2Layout->addWidget(c2Title);
    c2Layout->addWidget(categoryCountLabel);

    // Card 3 - Transactions
    QWidget *card3 = new QWidget();
    card3->setStyleSheet(cardStyle);
    QVBoxLayout *c3Layout = new QVBoxLayout(card3);
    QLabel *c3Title = new QLabel("Transactions");
    c3Title->setStyleSheet(subtitleStyle);
    transactionCountLabel = new QLabel("0");
    transactionCountLabel->setStyleSheet(valueStyle);
    c3Layout->addWidget(c3Title);
    c3Layout->addWidget(transactionCountLabel);

    // Card 4 - Total Income
    QWidget *card4 = new QWidget();
    card4->setStyleSheet(cardStyle);
    QVBoxLayout *c4Layout = new QVBoxLayout(card4);
    QLabel *c4Title = new QLabel("Total Income");
    c4Title->setStyleSheet(subtitleStyle);
    totalIncomeLabel = new QLabel("0 VND");
    totalIncomeLabel->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #0be881;"
        "background-color: transparent; border: none; padding: 0px;");
    c4Layout->addWidget(c4Title);
    c4Layout->addWidget(totalIncomeLabel);

    // Card 5 - Total Expense
    QWidget *card5 = new QWidget();
    card5->setStyleSheet(cardStyle);
    QVBoxLayout *c5Layout = new QVBoxLayout(card5);
    QLabel *c5Title = new QLabel("Total Expense");
    c5Title->setStyleSheet(subtitleStyle);
    totalExpenseLabel = new QLabel("0 VND");
    totalExpenseLabel->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #ff3f34;"
        "background-color: transparent; border: none; padding: 0px;");
    c5Layout->addWidget(c5Title);
    c5Layout->addWidget(totalExpenseLabel);

    // Card 6 - Balance
    QWidget *card6 = new QWidget();
    card6->setStyleSheet(cardStyle);
    QVBoxLayout *c6Layout = new QVBoxLayout(card6);
    QLabel *c6Title = new QLabel("Balance");
    c6Title->setStyleSheet(subtitleStyle);
    balanceLabel = new QLabel("0 VND");
    balanceLabel->setStyleSheet(
        "font-size: 20px; font-weight: bold; color: #3742fa;"
        "background-color: transparent; border: none; padding: 0px;");
    c6Layout->addWidget(c6Title);
    c6Layout->addWidget(balanceLabel);

    grid->addWidget(card1, 0, 0);
    grid->addWidget(card2, 0, 1);
    grid->addWidget(card3, 0, 2);
    grid->addWidget(card4, 1, 0);
    grid->addWidget(card5, 1, 1);
    grid->addWidget(card6, 1, 2);

    // Nút refresh
    QPushButton *btnRefresh = new QPushButton("Refresh Data");
    btnRefresh->setFixedWidth(150);
    connect(btnRefresh, &QPushButton::clicked,
            this, &DashboardPage::loadData);

    layout->addWidget(title);
    layout->addLayout(grid);
    layout->addWidget(btnRefresh);
    layout->addStretch();

    setLayout(layout);

    // Load dữ liệu
    loadData();
}

void DashboardPage::loadData()
{
    QVector<Account> accounts =
        AppContext::instance().accountRepository().getAllAccounts();
    QVector<Category> categories =
        AppContext::instance().categoryRepository().getAllCategories();
    QVector<Transaction> transactions =
        AppContext::instance().transactionRepository().getAllTransactions();

    accountCountLabel->setText(QString::number(accounts.size()));
    categoryCountLabel->setText(QString::number(categories.size()));
    transactionCountLabel->setText(QString::number(transactions.size()));

    double totalIncome = 0;
    double totalExpense = 0;

    for (const Transaction &t : transactions) {
        if (t.getType() == "income") {
            totalIncome += t.getAmount();
        } else if (t.getType() == "expense") {
            totalExpense += t.getAmount();
        }
    }

    QLocale locale(QLocale::Vietnamese, QLocale::Vietnam);
    totalIncomeLabel->setText(
        locale.toString(totalIncome, 'f', 0) + " VND");
    totalExpenseLabel->setText(
        locale.toString(totalExpense, 'f', 0) + " VND");
    balanceLabel->setText(
        locale.toString(totalIncome - totalExpense, 'f', 0) + " VND");
}
