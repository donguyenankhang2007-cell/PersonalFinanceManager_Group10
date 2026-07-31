#include "TransactionPage.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTableWidget>
#include <QHeaderView>

TransactionPage::TransactionPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Tiêu đề
    QLabel *title = new QLabel("Transaction History");

    // Nút thêm giao dịch
    QPushButton *btnAddTransaction =
        new QPushButton("Add Transaction");

    // Bảng giao dịch
    QTableWidget *table = new QTableWidget();

    table->setColumnCount(4);
    table->setHorizontalHeaderLabels(
        {"Date", "Category", "Amount", "Type"});

    table->horizontalHeader()->setStretchLastSection(true);
    table->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);

    // Thêm các widget vào layout
    layout->addWidget(title);
    layout->addWidget(btnAddTransaction);
    layout->addWidget(table);

    setLayout(layout);
}
