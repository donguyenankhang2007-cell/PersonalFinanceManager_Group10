#include "AccountPage.h"
#include "../dialogs/AccountDialog.h"
#include "../../app/AppContext.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>
#include <QLocale>

AccountPage::AccountPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(15);

    // Tiêu đề
    QLabel *title = new QLabel("Account Management");
    title->setStyleSheet(
        "font-size: 22px;"
        "font-weight: bold;"
        "color: #1e272e;"
        "padding-bottom: 10px;");

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnAdd = new QPushButton("+ Add Account");
    btnDelete = new QPushButton("Delete Selected");
    btnDelete->setStyleSheet(
        "background-color: #ff3f34; color: white;"
        "border: none; padding: 10px 20px;"
        "font-size: 13px; font-weight: bold; border-radius: 5px;");
    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnDelete);
    btnLayout->addStretch();

    // Bảng danh sách accounts
    table = new QTableWidget();
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels(
        {"ID", "Name", "Balance (VND)", "Description"});
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
            this, &AccountPage::onAddAccount);
    connect(btnDelete, &QPushButton::clicked,
            this, &AccountPage::onDeleteAccount);

    // Load dữ liệu
    loadAccounts();
}

void AccountPage::loadAccounts()
{
    table->setRowCount(0);

    QVector<Account> accounts =
        AppContext::instance().accountRepository().getAllAccounts();

    QLocale locale(QLocale::Vietnamese, QLocale::Vietnam);

    for (const Account &acc : accounts) {
        int row = table->rowCount();
        table->insertRow(row);

        table->setItem(row, 0,
                       new QTableWidgetItem(QString::number(acc.getId())));
        table->setItem(row, 1,
                       new QTableWidgetItem(acc.getName()));
        table->setItem(row, 2,
                       new QTableWidgetItem(
                           locale.toString(acc.getBalance(), 'f', 0)));
        table->setItem(row, 3,
                       new QTableWidgetItem(acc.getDescription()));
    }
}

void AccountPage::onAddAccount()
{
    AccountDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        Account acc = dialog.getAccount();

        if (acc.getName().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Error",
                                 "Account name cannot be empty!");
            return;
        }

        if (AppContext::instance().accountRepository().addAccount(acc)) {
            loadAccounts();
        } else {
            QMessageBox::warning(this, "Error",
                                 "Failed to add account!");
        }
    }
}

void AccountPage::onDeleteAccount()
{
    int row = table->currentRow();

    if (row < 0) {
        QMessageBox::information(this, "Info",
                                 "Please select an account to delete.");
        return;
    }

    int id = table->item(row, 0)->text().toInt();
    QString name = table->item(row, 1)->text();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm",
        QString("Are you sure you want to delete account '%1'?\n"
                "All transactions linked to this account may be affected.")
            .arg(name),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (AppContext::instance().accountRepository().deleteAccount(id)) {
            loadAccounts();
        } else {
            QMessageBox::warning(this, "Error",
                                 "Failed to delete account!");
        }
    }
}
