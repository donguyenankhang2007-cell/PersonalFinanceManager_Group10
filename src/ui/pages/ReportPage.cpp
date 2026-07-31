// ============================================
// File: src/ui/pages/ReportPage.cpp
// Nguoi viet: Viet Tuong (GUI Developer)
// Mo ta: Trang bao cao tai chinh —
//        thong ke thu/chi theo thang, so sanh voi thang truoc
//        Ket noi ReportService, TransactionRepository
// ============================================
#include "ReportPage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QFrame>
#include <QDate>
#include <QPushButton>
#include <cmath>

#include "core/repositories/TransactionRepository.h"
#include "core/services/ReportService.h"
#include "core/models/Report.h"
#include "utils/MoneyUtils.h"
#include "utils/DateUtils.h"

ReportPage::ReportPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

// ==================== SETUP GIAO DIEN ====================
void ReportPage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(20);

    // Tieu de
    QLabel *title = new QLabel("Bao cao Tai chinh");
    title->setStyleSheet("color: #1A237E; font-size: 22px; font-weight: bold;");
    QLabel *subtitle = new QLabel("Thong ke thu chi theo thang va so sanh");
    subtitle->setStyleSheet("color: #757575; font-size: 13px;");
    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    // === BO LOC THOI GIAN ===
    QHBoxLayout *filterLayout = new QHBoxLayout();

    QLabel *monthLabel = new QLabel("Thang:");
    monthLabel->setStyleSheet("font-weight: bold; color: #424242;");
    m_monthSpin = new QSpinBox();
    m_monthSpin->setRange(1, 12);
    m_monthSpin->setValue(QDate::currentDate().month());

    QLabel *yearLabel = new QLabel("Nam:");
    yearLabel->setStyleSheet("font-weight: bold; color: #424242;");
    m_yearSpin = new QSpinBox();
    m_yearSpin->setRange(2020, 2030);
    m_yearSpin->setValue(QDate::currentDate().year());

    QPushButton *btnGenerate = new QPushButton("Xem bao cao");
    btnGenerate->setObjectName("primaryBtn");
    btnGenerate->setCursor(Qt::PointingHandCursor);
    connect(btnGenerate, &QPushButton::clicked,
            this, &ReportPage::generateReport);

    filterLayout->addWidget(monthLabel);
    filterLayout->addWidget(m_monthSpin);
    filterLayout->addSpacing(15);
    filterLayout->addWidget(yearLabel);
    filterLayout->addWidget(m_yearSpin);
    filterLayout->addSpacing(15);
    filterLayout->addWidget(btnGenerate);
    filterLayout->addStretch();
    mainLayout->addLayout(filterLayout);

    // === 4 THE BAO CAO ===
    QGridLayout *cardsGrid = new QGridLayout();
    cardsGrid->setSpacing(15);

    // The 1: Tong thu nhap
    QFrame *incomeCard = new QFrame();
    incomeCard->setStyleSheet(
        "QFrame { background: white; border: 1px solid #E8EAF6; "
        "border-radius: 10px; padding: 22px; border-top: 3px solid #2E7D32; }");
    QVBoxLayout *incomeLayout = new QVBoxLayout(incomeCard);
    QLabel *incomeTitle = new QLabel("TONG THU NHAP");
    incomeTitle->setStyleSheet(
        "color: #757575; font-size: 11px; font-weight: 600; letter-spacing: 0.5px;");
    m_incomeValue = new QLabel("0 VND");
    m_incomeValue->setStyleSheet(
        "font-size: 26px; font-weight: bold; color: #2E7D32;");
    incomeLayout->addWidget(incomeTitle);
    incomeLayout->addWidget(m_incomeValue);
    cardsGrid->addWidget(incomeCard, 0, 0);

    // The 2: Tong chi tieu
    QFrame *expenseCard = new QFrame();
    expenseCard->setStyleSheet(
        "QFrame { background: white; border: 1px solid #E8EAF6; "
        "border-radius: 10px; padding: 22px; border-top: 3px solid #C62828; }");
    QVBoxLayout *expenseLayout = new QVBoxLayout(expenseCard);
    QLabel *expenseTitle = new QLabel("TONG CHI TIEU");
    expenseTitle->setStyleSheet(
        "color: #757575; font-size: 11px; font-weight: 600; letter-spacing: 0.5px;");
    m_expenseValue = new QLabel("0 VND");
    m_expenseValue->setStyleSheet(
        "font-size: 26px; font-weight: bold; color: #C62828;");
    expenseLayout->addWidget(expenseTitle);
    expenseLayout->addWidget(m_expenseValue);
    cardsGrid->addWidget(expenseCard, 0, 1);

    // The 3: So du con lai
    QFrame *balanceCard = new QFrame();
    balanceCard->setStyleSheet(
        "QFrame { background: white; border: 1px solid #E8EAF6; "
        "border-radius: 10px; padding: 22px; border-top: 3px solid #1565C0; }");
    QVBoxLayout *balanceLayout = new QVBoxLayout(balanceCard);
    QLabel *balanceTitle = new QLabel("SO DU CON LAI");
    balanceTitle->setStyleSheet(
        "color: #757575; font-size: 11px; font-weight: 600; letter-spacing: 0.5px;");
    m_balanceValue = new QLabel("0 VND");
    m_balanceValue->setStyleSheet(
        "font-size: 26px; font-weight: bold; color: #1565C0;");
    balanceLayout->addWidget(balanceTitle);
    balanceLayout->addWidget(m_balanceValue);
    cardsGrid->addWidget(balanceCard, 1, 0);

    // The 4: So sanh voi thang truoc
    QFrame *compareCard = new QFrame();
    compareCard->setStyleSheet(
        "QFrame { background: white; border: 1px solid #E8EAF6; "
        "border-radius: 10px; padding: 22px; border-top: 3px solid #6A1B9A; }");
    QVBoxLayout *compareLayout = new QVBoxLayout(compareCard);
    QLabel *compareTitle = new QLabel("SO SANH VOI THANG TRUOC");
    compareTitle->setStyleSheet(
        "color: #757575; font-size: 11px; font-weight: 600; letter-spacing: 0.5px;");
    m_comparisonValue = new QLabel("0%");
    m_comparisonValue->setStyleSheet(
        "font-size: 26px; font-weight: bold; color: #6A1B9A;");
    compareLayout->addWidget(compareTitle);
    compareLayout->addWidget(m_comparisonValue);
    cardsGrid->addWidget(compareCard, 1, 1);

    mainLayout->addLayout(cardsGrid);
    mainLayout->addStretch();
}

// ==================== TAO BAO CAO ====================
void ReportPage::generateReport()
{
    TransactionRepository transRepo;
    ReportService reportService;

    int month = m_monthSpin->value();
    int year  = m_yearSpin->value();

    // Khoang thoi gian thang hien tai
    QDate fromDate = DateUtils::firstDayOfMonth(year, month);
    QDate toDate   = DateUtils::lastDayOfMonth(year, month);

    QVector<Transaction> transactions = transRepo.getAllTransactions();
    QList<Transaction> txList(transactions.begin(), transactions.end());

    // Tao bao cao (su dung ReportService)
    Report report = reportService.generateReport(txList, fromDate, toDate);

    // Cap nhat hien thi
    m_incomeValue->setText(MoneyUtils::formatVND(report.getTotalIncome()));
    m_expenseValue->setText(MoneyUtils::formatVND(report.getTotalExpense()));
    m_balanceValue->setText(MoneyUtils::formatVND(report.getRemainingBalance()));

    // Mau so du: xanh neu duong, do neu am
    if (report.getRemainingBalance() >= 0) {
        m_balanceValue->setStyleSheet(
            "font-size: 26px; font-weight: bold; color: #2E7D32;");
    } else {
        m_balanceValue->setStyleSheet(
            "font-size: 26px; font-weight: bold; color: #C62828;");
    }

    // So sanh voi thang truoc (su dung ReportService::compareExpense)
    int prevYear, prevMonth;
    DateUtils::previousMonth(year, month, prevYear, prevMonth);
    QDate prevFrom = DateUtils::firstDayOfMonth(prevYear, prevMonth);
    QDate prevTo   = DateUtils::lastDayOfMonth(prevYear, prevMonth);

    double changePercent = reportService.compareExpense(
        txList, fromDate, toDate, prevFrom, prevTo);

    // Mui ten len = chi tieu tang (do), mui ten xuong = chi tieu giam (xanh)
    QString arrow = (changePercent >= 0) ? "▲" : "▼";
    QString color = (changePercent >= 0) ? "#E53935" : "#2E7D32";

    m_comparisonValue->setText(
        QString("%1 %2%").arg(arrow).arg(
            QString::number(std::abs(changePercent), 'f', 1)));
    m_comparisonValue->setStyleSheet(
        QString("font-size: 26px; font-weight: bold; color: %1;").arg(color));
}
