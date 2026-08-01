#include "TransactionPage.h"
#include "../dialogs/TransactionDialog.h"
#include "../../app/AppContext.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>
#include <QMap>

TransactionPage::TransactionPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(15);

    // Tiêu đề
    QLabel *title = new QLabel("Transaction History");
    title->setStyleSheet(
        "font-size: 22px;"
        "font-weight: bold;"
        "color: #1e272e;"
        "padding-bottom: 10px;");

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnAdd = new QPushButton("+ Add Transaction");
    btnDelete = new QPushButton("Delete Selected");
    btnDelete->setStyleSheet(
        "background-color: #ff3f34; color: white;"
        "border: none; padding: 10px 20px;"
        "font-size: 13px; font-weight: bold; border-radius: 5px;");
    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnDelete);
    btnLayout->addStretch();

    // Bảng giao dịch
    table = new QTableWidget();
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels(
        {"ID", "Date", "Account", "Category", "Amount (VND)", "Type"});
    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);

    layout->addWidget(title);
    layout->addLayout(btnLayout);
    layout->addWidget(table);

    setLayout(layout);

    // Kết nối signals
    connect(btnAdd, &QPushButton::clicked,
            this, &TransactionPage::onAddTransaction);
    connect(btnDelete, &QPushButton::clicked,
            this, &TransactionPage::onDeleteTransaction);

    // Load dữ liệu
    loadTransactions();
}

void TransactionPage::loadTransactions()
{
    table->setRowCount(0);

    QVector<Transaction> transactions =
        AppContext::instance().transactionRepository().getAllTransactions();

    // Load category names for display
    QVector<Category> categories =
        AppContext::instance().categoryRepository().getAllCategories();
    QMap<int, QString> categoryNames;
    for (const Category &c : categories) {
        categoryNames[c.getId()] = c.getName();
    }

    // Load account names for display
    QVector<Account> accounts =
        AppContext::instance().accountRepository().getAllAccounts();
    QMap<int, QString> accountNames;
    for (const Account &a : accounts) {
        accountNames[a.getId()] = a.getName();
    }

    for (const Transaction &t : transactions) {
        int row = table->rowCount();
        table->insertRow(row);

        table->setItem(row, 0,
                       new QTableWidgetItem(QString::number(t.getId())));
        table->setItem(row, 1,
                       new QTableWidgetItem(
                           t.getDate().toString("dd/MM/yyyy")));
        table->setItem(row, 2,
                       new QTableWidgetItem(
                           accountNames.value(t.getAccountId(),
                                              "Unknown")));
        table->setItem(row, 3,
                       new QTableWidgetItem(
                           categoryNames.value(t.getCategoryId(),
                                               "Unknown")));
        table->setItem(row, 4,
                       new QTableWidgetItem(
                           QString::number(t.getAmount(), 'f', 0)));
        table->setItem(row, 5,
                       new QTableWidgetItem(t.getType()));
    }
}

void TransactionPage::onAddTransaction()
{
    TransactionDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        Transaction t = dialog.getTransaction();

        if (t.getAmount() <= 0) {
            QMessageBox::warning(this, "Error",
                                 "Amount must be greater than 0!");
            return;
        }

        if (t.getAccountId() <= 0) {
            QMessageBox::warning(this, "Error",
                                 "Please select an account! "
                                 "You need to have at least one account.");
            return;
        }

        if (t.getCategoryId() <= 0) {
            QMessageBox::warning(this, "Error",
                                 "Please select a category! "
                                 "Add categories first.");
            return;
        }

        if (AppContext::instance()
                .transactionRepository()
                .addTransaction(t)) {
            loadTransactions();
        } else {
            QMessageBox::warning(this, "Error",
                                 "Failed to add transaction!");
        }
    }
}

void TransactionPage::onDeleteTransaction()
{
    int row = table->currentRow();

    if (row < 0) {
        QMessageBox::information(this, "Info",
                                 "Please select a transaction to delete.");
        return;
    }

    int id = table->item(row, 0)->text().toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm",
        "Are you sure you want to delete this transaction?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (AppContext::instance()
                .transactionRepository()
                .deleteTransaction(id)) {
            loadTransactions();
        } else {
            QMessageBox::warning(this, "Error",
                                 "Failed to delete transaction!");
        }
    }
}
