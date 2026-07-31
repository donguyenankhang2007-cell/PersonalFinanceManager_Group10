// ============================================
// File: src/ui/pages/DashboardPage.cpp
// Nguoi viet: Viet Tuong (GUI Developer)
// Mo ta: Trang tong quan tai chinh —
//        hien thi tong thu/chi/so du va giao dich gan day
// ============================================
#include "DashboardPage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QHeaderView>
#include <QDate>
#include <QMap>
#include <QFont>

#include "core/repositories/TransactionRepository.h"
#include "core/repositories/CategoryRepository.h"
#include "core/services/ReportService.h"
#include "utils/MoneyUtils.h"
#include "utils/DateUtils.h"

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

// ==================== SETUP GIAO DIEN ====================
void DashboardPage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(20);

    // Tieu de trang
    QLabel *title = new QLabel("Dashboard");
    title->setStyleSheet("color: #1A237E; font-size: 22px; font-weight: bold;");
    QLabel *subtitle = new QLabel("Tong quan tai chinh cua ban trong thang nay");
    subtitle->setStyleSheet("color: #757575; font-size: 13px;");
    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    // === 4 THE TONG QUAN ===
    QHBoxLayout *cardsLayout = new QHBoxLayout();
    cardsLayout->setSpacing(15);

    // The 1: Tong thu nhap
    m_totalIncomeValue = new QLabel("0 VND");
    cardsLayout->addWidget(
        createSummaryCard("TONG THU NHAP", m_totalIncomeValue, "#2E7D32"));

    // The 2: Tong chi tieu
    m_totalExpenseValue = new QLabel("0 VND");
    cardsLayout->addWidget(
        createSummaryCard("TONG CHI TIEU", m_totalExpenseValue, "#C62828"));

    // The 3: So du
    m_balanceValue = new QLabel("0 VND");
    cardsLayout->addWidget(
        createSummaryCard("SO DU", m_balanceValue, "#1565C0"));

    // The 4: So luong giao dich
    m_transactionCountValue = new QLabel("0");
    cardsLayout->addWidget(
        createSummaryCard("SO GIAO DICH", m_transactionCountValue, "#6A1B9A"));

    mainLayout->addLayout(cardsLayout);

    // === BANG GIAO DICH GAN DAY ===
    QLabel *recentTitle = new QLabel("Giao dich gan day");
    recentTitle->setStyleSheet(
        "font-size: 16px; font-weight: bold; color: #1A237E; padding-top: 10px;");
    mainLayout->addWidget(recentTitle);

    m_recentTable = new QTableWidget();
    m_recentTable->setColumnCount(5);
    m_recentTable->setHorizontalHeaderLabels(
        {"Ngay", "Loai", "Danh muc", "Ghi chu", "So tien"});
    m_recentTable->horizontalHeader()->setStretchLastSection(true);
    m_recentTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_recentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_recentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_recentTable->verticalHeader()->setVisible(false);
    m_recentTable->setAlternatingRowColors(true);
    m_recentTable->setStyleSheet("alternate-background-color: #F8F9FA;");
    mainLayout->addWidget(m_recentTable, 1);
}

// ==================== TAO THE TONG QUAN ====================
QWidget* DashboardPage::createSummaryCard(const QString &title,
                                            QLabel *valueLabel,
                                            const QString &accentColor)
{
    QFrame *card = new QFrame();
    card->setStyleSheet(QString(
        "QFrame { background: white; border: 1px solid #E8EAF6; "
        "border-radius: 10px; padding: 18px; border-top: 3px solid %1; }"
    ).arg(accentColor));

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setSpacing(8);

    QLabel *titleLabel = new QLabel(title);
    titleLabel->setStyleSheet(
        "color: #757575; font-size: 11px; font-weight: 600; letter-spacing: 0.5px;");

    valueLabel->setStyleSheet(QString(
        "font-size: 22px; font-weight: bold; color: %1;").arg(accentColor));

    layout->addWidget(titleLabel);
    layout->addWidget(valueLabel);

    return card;
}

// ==================== LAM MOI DU LIEU ====================
void DashboardPage::refreshData()
{
    // Lay du lieu tu Repository va Service
    TransactionRepository transRepo;
    CategoryRepository catRepo;
    ReportService reportService;

    QVector<Transaction> transactions = transRepo.getAllTransactions();
    QVector<Category> categories = catRepo.getAllCategories();

    // Tao bang tra cuu ten danh muc theo ID
    QMap<int, QString> categoryNames;
    for (const Category &cat : categories) {
        categoryNames[cat.getId()] = cat.getName();
    }

    // Tinh tong thu/chi cho thang hien tai
    QDate today = QDate::currentDate();
    QDate monthStart = DateUtils::firstDayOfMonth(today.year(), today.month());
    QDate monthEnd = DateUtils::lastDayOfMonth(today.year(), today.month());

    // Chuyen QVector sang QList de truyen vao ReportService
    QList<Transaction> txList(transactions.begin(), transactions.end());

    double totalIncome  = reportService.getTotalIncome(txList, monthStart, monthEnd);
    double totalExpense = reportService.getTotalExpense(txList, monthStart, monthEnd);
    double balance      = totalIncome - totalExpense;

    // Cap nhat cac the tong quan
    m_totalIncomeValue->setText(MoneyUtils::formatVND(totalIncome));
    m_totalExpenseValue->setText(MoneyUtils::formatVND(totalExpense));
    m_balanceValue->setText(MoneyUtils::formatVND(balance));
    m_transactionCountValue->setText(QString::number(transactions.size()));

    // Hien thi 10 giao dich gan nhat
    int rowCount = qMin(transactions.size(), 10);
    m_recentTable->setRowCount(rowCount);

    for (int i = 0; i < rowCount; ++i) {
        // Lay tu cuoi danh sach (moi nhat truoc)
        int idx = transactions.size() - 1 - i;
        const Transaction &t = transactions[idx];

        // Cot Ngay
        m_recentTable->setItem(i, 0, new QTableWidgetItem(
            DateUtils::formatDisplay(t.getDate())));

        // Cot Loai (mau xanh cho thu nhap, do cho chi tieu)
        QString typeText = (t.getType() == "income") ? "Thu nhap" : "Chi tieu";
        QTableWidgetItem *typeItem = new QTableWidgetItem(typeText);
        typeItem->setForeground(
            t.getType() == "income" ? QColor("#2E7D32") : QColor("#C62828"));
        QFont boldFont;
        boldFont.setBold(true);
        typeItem->setFont(boldFont);
        m_recentTable->setItem(i, 1, typeItem);

        // Cot Danh muc
        QString catName = categoryNames.value(t.getCategoryId(), "Khong ro");
        m_recentTable->setItem(i, 2, new QTableWidgetItem(catName));

        // Cot Ghi chu
        m_recentTable->setItem(i, 3, new QTableWidgetItem(t.getNote()));

        // Cot So tien (canh phai, co mau)
        QTableWidgetItem *amountItem = new QTableWidgetItem(
            MoneyUtils::formatVND(t.getAmount()));
        amountItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        amountItem->setForeground(
            t.getType() == "income" ? QColor("#2E7D32") : QColor("#C62828"));
        m_recentTable->setItem(i, 4, amountItem);
    }
}
