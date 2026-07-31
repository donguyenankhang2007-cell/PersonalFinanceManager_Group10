#include "ReportPage.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QGroupBox>

ReportPage::ReportPage(QWidget *parent)
    : QWidget(parent)
{
    // Layout chính
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Tiêu đề
    QLabel *title = new QLabel("Monthly Financial Report");

    // Các thông tin báo cáo
    QLabel *incomeLabel =
        new QLabel("Total Income: 0 VND");

    QLabel *expenseLabel =
        new QLabel("Total Expense: 0 VND");

    QLabel *savingLabel =
        new QLabel("Total Saving: 0 VND");

    QLabel *balanceLabel =
        new QLabel("Current Balance: 0 VND");

    // Thêm vào layout
    layout->addWidget(title);
    layout->addWidget(incomeLabel);
    layout->addWidget(expenseLabel);
    layout->addWidget(savingLabel);
    layout->addWidget(balanceLabel);

    setLayout(layout);
}
