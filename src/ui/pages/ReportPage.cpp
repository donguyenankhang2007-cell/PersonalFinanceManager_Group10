#include "ReportPage.h"
#include "../../app/AppContext.h"
#include "../../utils/DateUtils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QPushButton>
#include <QDate>
#include <QLocale>

ReportPage::ReportPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(15);

    // Tiêu đề
    QLabel *title = new QLabel("Monthly Financial Report");
    title->setStyleSheet(
        "font-size: 22px;"
        "font-weight: bold;"
        "color: #1e272e;"
        "padding-bottom: 10px;");

    // === Filter ===
    QHBoxLayout *filterLayout = new QHBoxLayout();
    QLabel *monthLabel = new QLabel("Month:");
    monthLabel->setStyleSheet("font-weight: bold;");
    monthSpin = new QSpinBox();
    monthSpin->setRange(1, 12);
    monthSpin->setValue(QDate::currentDate().month());

    QLabel *yearLabel = new QLabel("Year:");
    yearLabel->setStyleSheet("font-weight: bold;");
    yearSpin = new QSpinBox();
    yearSpin->setRange(2020, 2030);
    yearSpin->setValue(QDate::currentDate().year());

    QPushButton *btnGenerate = new QPushButton("Generate Report");
    btnGenerate->setFixedWidth(150);

    filterLayout->addWidget(monthLabel);
    filterLayout->addWidget(monthSpin);
    filterLayout->addWidget(yearLabel);
    filterLayout->addWidget(yearSpin);
    filterLayout->addWidget(btnGenerate);
    filterLayout->addStretch();

    // === Kết quả ===
    QString cardStyle =
        "font-size: 16px;"
        "color: #2f3640;"
        "background-color: #ffffff;"
        "padding: 18px 22px;"
        "border-radius: 8px;"
        "border: 1px solid #dcdde1;";

    incomeLabel = new QLabel("Total Income:   0 VND");
    incomeLabel->setStyleSheet(cardStyle +
        "color: #0be881; font-weight: bold;");

    expenseLabel = new QLabel("Total Expense:  0 VND");
    expenseLabel->setStyleSheet(cardStyle +
        "color: #ff3f34; font-weight: bold;");

    balanceLabel = new QLabel("Balance:        0 VND");
    balanceLabel->setStyleSheet(cardStyle +
        "color: #3742fa; font-weight: bold;");

    compareLabel = new QLabel("Compared to last month:  N/A");
    compareLabel->setStyleSheet(cardStyle +
        "color: #485460;");

    layout->addWidget(title);
    layout->addLayout(filterLayout);
    layout->addWidget(incomeLabel);
    layout->addWidget(expenseLabel);
    layout->addWidget(balanceLabel);
    layout->addWidget(compareLabel);
    layout->addStretch();

    setLayout(layout);

    // Kết nối
    connect(btnGenerate, &QPushButton::clicked,
            this, &ReportPage::loadReport);

    // Load dữ liệu ban đầu
    loadReport();
}

void ReportPage::loadReport()
{
    int month = monthSpin->value();
    int year = yearSpin->value();

    QDate fromDate = DateUtils::firstDayOfMonth(year, month);
    QDate toDate = DateUtils::lastDayOfMonth(year, month);

    // Lấy tất cả transactions
    QVector<Transaction> allTx =
        AppContext::instance().transactionRepository().getAllTransactions();

    // Chuyển sang QList cho ReportService
    QList<Transaction> txList(allTx.begin(), allTx.end());

    // Dùng ReportService
    ReportService &reportService =
        AppContext::instance().reportService();

    Report report = reportService.generateReport(txList, fromDate, toDate);

    QLocale locale(QLocale::Vietnamese, QLocale::Vietnam);

    incomeLabel->setText(QString("Total Income:   %1 VND")
                             .arg(locale.toString(
                                 report.getTotalIncome(), 'f', 0)));

    expenseLabel->setText(QString("Total Expense:  %1 VND")
                              .arg(locale.toString(
                                  report.getTotalExpense(), 'f', 0)));

    balanceLabel->setText(QString("Balance:        %1 VND")
                              .arg(locale.toString(
                                  report.getRemainingBalance(), 'f', 0)));

    // So sánh với tháng trước
    int prevYear, prevMonth;
    DateUtils::previousMonth(year, month, prevYear, prevMonth);
    QDate prevFrom = DateUtils::firstDayOfMonth(prevYear, prevMonth);
    QDate prevTo = DateUtils::lastDayOfMonth(prevYear, prevMonth);

    double comparePercent = reportService.compareExpense(
        txList, fromDate, toDate, prevFrom, prevTo);

    QString compareText;
    if (comparePercent > 0) {
        compareText = QString("Compared to last month:  +%1% (more spending)")
                          .arg(QString::number(comparePercent, 'f', 1));
        compareLabel->setStyleSheet(
            "font-size: 16px; color: #ff3f34; font-weight: bold;"
            "background-color: #ffffff; padding: 18px 22px;"
            "border-radius: 8px; border: 1px solid #dcdde1;");
    } else if (comparePercent < 0) {
        compareText = QString("Compared to last month:  %1% (less spending)")
                          .arg(QString::number(comparePercent, 'f', 1));
        compareLabel->setStyleSheet(
            "font-size: 16px; color: #0be881; font-weight: bold;"
            "background-color: #ffffff; padding: 18px 22px;"
            "border-radius: 8px; border: 1px solid #dcdde1;");
    } else {
        compareText = "Compared to last month:  No change";
        compareLabel->setStyleSheet(
            "font-size: 16px; color: #485460;"
            "background-color: #ffffff; padding: 18px 22px;"
            "border-radius: 8px; border: 1px solid #dcdde1;");
    }
    compareLabel->setText(compareText);
}
