// ============================================
// File: src/ui/pages/DashboardPage.cpp
// Mo ta: Trang Dashboard — the thong ke, thu/chi thang,
//        canh bao ngan sach, bieu do (Qt Charts), top danh muc,
//        giao dich gan day. Toan bo hien thi theo ThemeManager.
// ============================================
#include "DashboardPage.h"

#include "../../app/AppContext.h"
#include "../../utils/MoneyUtils.h"
#include "../../utils/DateUtils.h"
#include "../theme/ThemeManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QTableWidget>
#include <QHeaderView>
#include <QMenu>
#include <QAction>

#include "../../utils/StyleUtils.h"
#include <QDate>
#include <QMap>
#include <algorithm>
#include <QToolTip>
#include <QCursor>
#include <QProgressBar>
#include <QComboBox>
#include <QGraphicsTextItem>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QLineSeries>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QCategoryAxis>
#include <QtCharts/QValueAxis>

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent)
{
    buildLayout();

    // Observer: tự cập nhật khi dữ liệu thay đổi ở nơi khác
    TransactionService &txService = AppContext::instance().transactionService();
    connect(&txService, &TransactionService::transactionAdded,
            this, &DashboardPage::loadData);
    connect(&txService, &TransactionService::transactionUpdated,
            this, &DashboardPage::loadData);
    connect(&txService, &TransactionService::transactionRemoved,
            this, &DashboardPage::loadData);

    // Ẩn/hiện biểu đồ theo cài đặt Settings + đổi accent → vẽ lại
    ThemeManager &theme = ThemeManager::instance();
    connect(&theme, &ThemeManager::chartsEnabledChanged,
            this, &DashboardPage::loadData);
    connect(&theme, &ThemeManager::themeChanged,
            this, &DashboardPage::loadData);

    loadData();
}

// ==================== LAYOUT ====================
void DashboardPage::buildLayout()
{
    QVBoxLayout *outer = new QVBoxLayout(this);
    outer->setContentsMargins(0, 0, 0, 0);
    outer->setSpacing(0);

    QScrollArea *scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    QWidget *content = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout(content);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(20);

    // Tiêu đề
    QHBoxLayout *header = new QHBoxLayout();
    QVBoxLayout *titleBox = new QVBoxLayout();
    titleBox->setSpacing(2);
    QLabel *title = new QLabel("Tổng Quan");
    title->setObjectName("pageTitle");
    QLabel *subtitle = new QLabel("Bức tranh tài chính của bạn");
    subtitle->setObjectName("pageSubtitle");
    titleBox->addWidget(title);
    titleBox->addWidget(subtitle);
    header->addLayout(titleBox);
    header->addStretch();
    layout->addLayout(header);

    // === Filter Bar (Month/Year + Export) ===
    QWidget *filterCard = new QWidget();
    filterCard->setObjectName("card");
    QHBoxLayout *filterLayout = new QHBoxLayout(filterCard);
    filterLayout->setContentsMargins(16, 14, 16, 14);
    filterLayout->setSpacing(10);

    QLabel *monthLabel = new QLabel("Tháng:");
    monthLabel->setObjectName("cardTitle");
    monthCb = new QComboBox();
    for (int i = 1; i <= 12; ++i) {
        monthCb->addItem(QString("Tháng %1").arg(i), i);
    }
    monthCb->setCurrentIndex(QDate::currentDate().month() - 1);

    QLabel *yearLabel = new QLabel("Năm:");
    yearLabel->setObjectName("cardTitle");
    yearCb = new QComboBox();
    for (int i = 2020; i <= 2030; ++i) {
        yearCb->addItem(QString::number(i), i);
    }
    yearCb->setCurrentText(QString::number(QDate::currentDate().year()));

    QPushButton *btnThisMonth = new QPushButton("Tháng Này");
    btnThisMonth->setCursor(Qt::PointingHandCursor);
    connect(btnThisMonth, &QPushButton::clicked, this, &DashboardPage::setThisMonth);
    
    QPushButton *btnLastMonth = new QPushButton("Tháng Trước");
    btnLastMonth->setCursor(Qt::PointingHandCursor);
    connect(btnLastMonth, &QPushButton::clicked, this, &DashboardPage::setLastMonth);

    QPushButton *btnExport = new QPushButton("Xuất CSV");
    btnExport->setObjectName("secondaryBtn");
    btnExport->setCursor(Qt::PointingHandCursor);
    connect(btnExport, &QPushButton::clicked, this, &DashboardPage::exportCsv);

    connect(monthCb, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DashboardPage::loadData);
    connect(yearCb, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &DashboardPage::loadData);

    filterLayout->addWidget(monthLabel);
    filterLayout->addWidget(monthCb);
    filterLayout->addWidget(yearLabel);
    filterLayout->addWidget(yearCb);
    filterLayout->addSpacing(10);
    filterLayout->addWidget(btnThisMonth);
    filterLayout->addWidget(btnLastMonth);
    filterLayout->addStretch();
    filterLayout->addWidget(btnExport);
    layout->addWidget(filterCard);

    // === 1. Thẻ Balance ===
    QWidget *balanceCard = new QWidget();
    balanceCard->setObjectName("card");
    balanceCard->setStyleSheet("QWidget#card { background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #3b82f6, stop:1 #2563eb); border: none; }");
    QVBoxLayout *balLayout = new QVBoxLayout(balanceCard);
    balLayout->setContentsMargins(24, 24, 24, 24);
    
    QLabel *balTitle = new QLabel("Tổng Số Dư");
    balTitle->setStyleSheet("color: rgba(255, 255, 255, 0.8); font-size: 14px; font-weight: 600; text-transform: uppercase; letter-spacing: 1px;");
    balanceLabel = new QLabel("0");
    balanceLabel->setStyleSheet("color: white; font-size: 36px; font-weight: 800;");
    compareBalanceLabel = new QLabel("");
    compareBalanceLabel->setStyleSheet("color: rgba(255, 255, 255, 0.9); font-size: 13px; font-weight: bold;");
    
    balLayout->addWidget(balTitle);
    balLayout->addWidget(balanceLabel);
    balLayout->addWidget(compareBalanceLabel);
    layout->addWidget(balanceCard);

    // === 2. Thẻ Thu nhập / Chi tiêu ===
    QGridLayout *financeGrid = new QGridLayout();
    financeGrid->setSpacing(15);
    QWidget *c4, *c5, *m1, *m2, *m3;
    
    // Total income/expense (we modify makeCard slightly to inject compare labels after)
    makeCard(c4, totalIncomeLabel, "Tổng Thu Nhập", "cardValueGreen");
    compareIncomeLabel = new QLabel("");
    compareIncomeLabel->setStyleSheet("font-size: 13px; font-weight: bold;");
    c4->layout()->addWidget(compareIncomeLabel);

    makeCard(c5, totalExpenseLabel, "Tổng Chi Tiêu", "cardValueRed");
    compareExpenseLabel = new QLabel("");
    compareExpenseLabel->setStyleSheet("font-size: 13px; font-weight: bold;");
    c5->layout()->addWidget(compareExpenseLabel);

    makeCard(m1, monthIncomeLabel, "Thu Nhập Tháng Này", "cardValueGreen");
    makeCard(m2, monthExpenseLabel, "Chi Tiêu Tháng Này", "cardValueRed");
    makeCard(m3, monthNetLabel, "Lưu Chuyển Thuần", "cardValueAccent");

    QHBoxLayout *totalRow = new QHBoxLayout();
    totalRow->setSpacing(15);
    totalRow->addWidget(c4, 1);
    totalRow->addWidget(c5, 1);
    layout->addLayout(totalRow);

    QHBoxLayout *monthRow = new QHBoxLayout();
    monthRow->setSpacing(15);
    monthRow->addWidget(m1, 1);
    monthRow->addWidget(m2, 1);
    monthRow->addWidget(m3, 1);
    layout->addLayout(monthRow);

    // === 3. Meta info ===
    QHBoxLayout *metaRow = new QHBoxLayout();
    metaRow->setSpacing(15);
    QWidget *c1, *c2, *c3;
    makeCompactCard(c1, accountCountLabel, "Tài Khoản");
    makeCompactCard(c2, categoryCountLabel, "Danh Mục");
    makeCompactCard(c3, transactionCountLabel, "Giao Dịch");
    metaRow->addWidget(c1, 1);
    metaRow->addWidget(c2, 1);
    metaRow->addWidget(c3, 1);
    layout->addLayout(metaRow);


    // === Biểu đồ ===
    chartsCard = new QWidget();
    chartsCard->setObjectName("card");
    QVBoxLayout *chartsLayout = new QVBoxLayout(chartsCard);
    chartsLayout->setContentsMargins(18, 18, 18, 18);
    chartsLayout->setSpacing(10);
    QLabel *chartsTitle = new QLabel("Phân Tích");
    chartsTitle->setObjectName("cardTitle");
    chartsLayout->addWidget(chartsTitle);

    QHBoxLayout *chartsRow = new QHBoxLayout();
    chartsRow->setSpacing(18);

    // Donut
    QHBoxLayout *donutCardLayout = new QHBoxLayout();
    donutView = new QChartView();
    donutView->setRenderHint(QPainter::Antialiasing);
    donutView->setMinimumSize(300, 300);
    
    donutLegendWidget = new QWidget();
    donutLegendLayout = new QVBoxLayout(donutLegendWidget);
    donutLegendLayout->setAlignment(Qt::AlignVCenter);
    donutLegendLayout->setSpacing(8);
    
    donutCenterText = new QGraphicsTextItem(donutView->chart());
    donutCenterText->setZValue(10);
    
    donutCardLayout->addWidget(donutView, 1);
    donutCardLayout->addWidget(donutLegendWidget);
    
    // Bar/Trend
    barView = new QChartView();
    barView->setRenderHint(QPainter::Antialiasing);
    barView->setMinimumSize(420, 300);
    barView->chart()->legend()->setVisible(true);
    barView->chart()->legend()->setAlignment(Qt::AlignBottom);

    chartsRow->addLayout(donutCardLayout, 1);
    chartsRow->addWidget(barView, 1);
    chartsLayout->addLayout(chartsRow);
    layout->addWidget(chartsCard);

    // === Hàng dưới: Top danh mục + Giao dịch gần đây ===
    QHBoxLayout *bottomRow = new QHBoxLayout();
    bottomRow->setSpacing(15);

    QWidget *topCard = new QWidget();
    topCard->setObjectName("card");
    QVBoxLayout *topLayout = new QVBoxLayout(topCard);
    topLayout->setContentsMargins(16, 14, 16, 14);
    topLayout->setSpacing(8);
    QLabel *topTitle = new QLabel("Top Danh Mục Chi Tiêu");
    topTitle->setObjectName("cardTitle");
    topLayout->addWidget(topTitle);
    topCategoriesTable = new QTableWidget();
    topCategoriesTable->setColumnCount(3);
    topCategoriesTable->setHorizontalHeaderLabels({"Danh Mục", "Đã Chi", "% Tổng"});
    topCategoriesTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    topCategoriesTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Fixed);
    topCategoriesTable->setColumnWidth(0, 180);
    topCategoriesTable->setMinimumHeight(240);
    topCategoriesTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    topCategoriesTable->setSelectionMode(QAbstractItemView::NoSelection);
    topCategoriesTable->setFocusPolicy(Qt::NoFocus);
    topCategoriesTable->verticalHeader()->setVisible(false);
    topLayout->addWidget(topCategoriesTable);

    QWidget *recentCard = new QWidget();
    recentCard->setObjectName("card");
    QVBoxLayout *recentLayout = new QVBoxLayout(recentCard);
    recentLayout->setContentsMargins(16, 14, 16, 14);
    recentLayout->setSpacing(8);
    QLabel *recentTitle = new QLabel("Giao Dịch Gần Đây");
    recentTitle->setObjectName("cardTitle");
    recentLayout->addWidget(recentTitle);
    recentTable = new QTableWidget();
    recentTable->setColumnCount(4);
    recentTable->setHorizontalHeaderLabels({"Ngày", "Danh Mục", "Ghi Chú", "Số Tiền"});
    recentTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    recentTable->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Fixed);
    recentTable->setColumnWidth(3, 130);
    recentTable->setMinimumHeight(240);
    recentTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    recentTable->setSelectionMode(QAbstractItemView::NoSelection);
    recentTable->setFocusPolicy(Qt::NoFocus);
    recentTable->verticalHeader()->setVisible(false);
    recentLayout->addWidget(recentTable);

    bottomRow->addWidget(topCard, 1);
    bottomRow->addWidget(recentCard, 1);
    layout->addLayout(bottomRow);

    scroll->setWidget(content);
    outer->addWidget(scroll);
}

void DashboardPage::makeCard(QWidget *&card, QLabel *&valueLabel,
                             const QString &title, const QString &valueObjectName)
{
    card = new QWidget();
    card->setObjectName("card");
    QVBoxLayout *cl = new QVBoxLayout(card);
    cl->setContentsMargins(18, 16, 18, 16);
    cl->setSpacing(6);

    QLabel *ct = new QLabel(title);
    ct->setObjectName("cardTitle");

    valueLabel = new QLabel("0");
    valueLabel->setObjectName(valueObjectName);

    cl->addWidget(ct);
    cl->addWidget(valueLabel);
}

void DashboardPage::makeCompactCard(QWidget *&card, QLabel *&valueLabel, const QString &title)
{
    card = new QWidget();
    card->setObjectName("card");
    QHBoxLayout *cl = new QHBoxLayout(card);
    cl->setContentsMargins(16, 12, 16, 12);
    
    QLabel *ct = new QLabel(title);
    ct->setObjectName("cardTitle");
    QFont f = ct->font();
    f.setPointSize(11);
    ct->setFont(f);
    
    valueLabel = new QLabel("0");
    valueLabel->setObjectName("cardValue");
    QFont f2 = valueLabel->font();
    f2.setPointSize(14);
    valueLabel->setFont(f2);
    
    cl->addWidget(ct);
    cl->addStretch();
    cl->addWidget(valueLabel);
}

void DashboardPage::setThisMonth()
{
    QDate now = QDate::currentDate();
    monthCb->setCurrentIndex(now.month() - 1);
    yearCb->setCurrentText(QString::number(now.year()));
}

void DashboardPage::setLastMonth()
{
    QDate now = QDate::currentDate();
    int m = now.month() - 1;
    int y = now.year();
    if (m == 1) {
        m = 12;
        y--;
    } else {
        m--;
    }
    monthCb->setCurrentIndex(m - 1);
    yearCb->setCurrentText(QString::number(y));
}

// ==================== DỮ LIỆU ====================
void DashboardPage::loadData()
{
    AppContext &ctx = AppContext::instance();
    QVector<Account> accounts = ctx.accountService().getAllAccounts();
    QVector<Category> categories = ctx.categoryService().getAllCategories();
    TransactionService &txService = ctx.transactionService();
    QVector<Transaction> allTransactions = txService.getAllTransactions();

    int month = monthCb->currentData().toInt();
    int year = yearCb->currentData().toInt();
    QDate fromDate = DateUtils::firstDayOfMonth(year, month);
    QDate toDate = DateUtils::lastDayOfMonth(year, month);

    QList<Transaction> allTxList(allTransactions.begin(), allTransactions.end());
    QList<Transaction> monthTxList = txService.filterByDateRange(allTxList, fromDate, toDate);

    accountCountLabel->setText(QString::number(accounts.size()));
    categoryCountLabel->setText(QString::number(categories.size()));
    transactionCountLabel->setText(QString::number(monthTxList.size()));

    // Tạo báo cáo cho tháng đã chọn
    ReportService &reportService = ctx.reportService();
    Report report = reportService.generateReport(allTxList, fromDate, toDate);

    totalIncomeLabel->setText(MoneyUtils::formatVND(report.getTotalIncome()));
    totalExpenseLabel->setText(MoneyUtils::formatVND(report.getTotalExpense()));
    
    double totalBalance = 0;
    for (const Account &acc : accounts) totalBalance += acc.getBalance();
    balanceLabel->setText(MoneyUtils::formatVND(totalBalance)); 
    
    monthIncomeLabel->setText(MoneyUtils::formatVND(report.getTotalIncome()));
    monthExpenseLabel->setText(MoneyUtils::formatVND(report.getTotalExpense()));
    monthNetLabel->setText(MoneyUtils::formatVND(report.getTotalIncome() - report.getTotalExpense()));

    // So sánh với tháng trước
    int prevYear, prevMonth;
    DateUtils::previousMonth(year, month, prevYear, prevMonth);
    QDate prevFrom = DateUtils::firstDayOfMonth(prevYear, prevMonth);
    QDate prevTo = DateUtils::lastDayOfMonth(prevYear, prevMonth);
    Report prevReport = reportService.generateReport(allTxList, prevFrom, prevTo);

    auto setCompareLabel = [](QLabel* label, double current, double prev, bool isExpense, bool isWhite = false) {
        if (prev == 0) {
            label->setText("Chưa có dữ liệu");
            label->setStyleSheet(isWhite ? "color: rgba(255, 255, 255, 0.7); font-size: 13px; font-weight: bold;" : "color: #94A3B8; font-size: 13px; font-weight: bold;");
            return;
        }
        double diff = current - prev;
        double pct = (diff / prev) * 100.0;
        
        QString text = QString("%1%2%").arg(pct > 0 ? "+" : "").arg(QString::number(pct, 'f', 1));
        QString color = isWhite ? "rgba(255, 255, 255, 0.9)" : "#94A3B8";
        
        if (diff > 0) {
            text = "⬆ " + text;
            if (!isWhite) color = isExpense ? "#E11D48" : "#059669";
        } else if (diff < 0) {
            text = "⬇ " + text;
            if (!isWhite) color = isExpense ? "#059669" : "#E11D48";
        } else {
            text = "Không thay đổi";
        }
        label->setText(text);
        label->setStyleSheet(QString("color: %1; font-size: 13px; font-weight: bold;").arg(color));
    };

    setCompareLabel(compareIncomeLabel, report.getTotalIncome(), prevReport.getTotalIncome(), false);
    setCompareLabel(compareExpenseLabel, report.getTotalExpense(), prevReport.getTotalExpense(), true);
    
    double netCurrent = report.getTotalIncome() - report.getTotalExpense();
    double netPrev = prevReport.getTotalIncome() - prevReport.getTotalExpense();
    setCompareLabel(compareBalanceLabel, netCurrent, netPrev, false, true);
    
    // === Top danh mục + giao dịch gần đây ===
    updateTopCategories(monthTxList);
    updateRecentTransactions(monthTxList);

    // === Biểu đồ ===
    bool chartsOn = ThemeManager::instance().chartsEnabled();
    chartsCard->setVisible(chartsOn);
    if (chartsOn) {
        updateCharts();
        updateTrendChart();
    }
}

// ==================== TOP DANH MỤC ====================
void DashboardPage::updateTopCategories(const QList<Transaction> &txList)
{
    topCategoriesTable->setRowCount(0);

    QVector<Category> categories = AppContext::instance().categoryService().getAllCategories();
    QMap<int, QString> catNames;
    QMap<int, QColor> catColors;
    for (const Category &c : categories) {
        catNames[c.getId()] = c.getName();
        catColors[c.getId()] = StyleUtils::getCategoryColor(c.getId());
    }

    QMap<int, double> expenseByCat;
    double totalMonthExpense = 0;
    for (const Transaction &t : txList) {
        if (t.isExpense()) {
            expenseByCat[t.getCategoryId()] += t.getAmount();
            totalMonthExpense += t.getAmount();
        }
    }

    QList<int> ids = expenseByCat.keys();
    std::sort(ids.begin(), ids.end(), [&expenseByCat](int a, int b) {
        return expenseByCat.value(a) > expenseByCat.value(b);
    });

    int rowHeight = ThemeManager::instance().tableRowHeight();
    int count = qMin(5, ids.size());
    for (int i = 0; i < count; ++i) {
        int catId = ids.at(i);
        double value = expenseByCat.value(catId);

        int row = topCategoriesTable->rowCount();
        topCategoriesTable->insertRow(row);
        topCategoriesTable->setRowHeight(row, rowHeight);

        QTableWidgetItem *nameItem = new QTableWidgetItem(QString("   %1").arg(catNames.value(catId, "Không xác định")));
        nameItem->setForeground(catColors.value(catId));
        topCategoriesTable->setItem(row, 0, nameItem);
        topCategoriesTable->setItem(row, 1, new QTableWidgetItem(MoneyUtils::formatVND(value)));

        double percent = totalMonthExpense > 0 ? value / totalMonthExpense * 100 : 0;
        QTableWidgetItem *percentItem = new QTableWidgetItem(QString("%1%").arg(QString::number(percent, 'f', 1)));
        percentItem->setForeground(QColor("#64748B"));
        topCategoriesTable->setItem(row, 2, percentItem);
    }
}

// ==================== GIAO DỊCH GẦN ĐÂY ====================
void DashboardPage::updateRecentTransactions(const QList<Transaction> &txList)
{
    recentTable->setRowCount(0);
    QVector<Category> categories = AppContext::instance().categoryService().getAllCategories();
    QMap<int, QString> catNames;
    for (const Category &c : categories) catNames[c.getId()] = c.getName();

    QList<Transaction> sorted = txList;
    std::sort(sorted.begin(), sorted.end(), [](const Transaction &a, const Transaction &b) {
        if (a.getDate() != b.getDate()) return a.getDate() > b.getDate();
        return a.getId() > b.getId();
    });

    int rowHeight = ThemeManager::instance().tableRowHeight();
    int count = qMin(10, sorted.size());
    for (int i = 0; i < count; ++i) {
        const Transaction &t = sorted.at(i);
        int row = recentTable->rowCount();
        recentTable->insertRow(row);
        recentTable->setRowHeight(row, rowHeight);

        recentTable->setItem(row, 0, new QTableWidgetItem(t.getDate().toString("dd/MM/yyyy")));
        recentTable->setItem(row, 1, new QTableWidgetItem(catNames.value(t.getCategoryId(), "Không xác định")));
        recentTable->setItem(row, 2, new QTableWidgetItem(t.getNote()));

        QTableWidgetItem *amountItem = new QTableWidgetItem(MoneyUtils::formatVND(t.getAmount()));
        amountItem->setForeground(t.isIncome() ? QColor("#059669") : QColor("#E11D48"));
        recentTable->setItem(row, 3, amountItem);
    }
}

// ==================== BIỂU ĐỒ ====================
void DashboardPage::setChartTheme()
{
    ThemeManager &theme = ThemeManager::instance();
    QColor text = theme.bodyTextColor();
    QColor titleColor = theme.textColor();
    QColor sub = theme.mutedColor();
    QColor grid = theme.gridColor();

    QChart *dChart = donutView->chart();
    dChart->setBackgroundVisible(false);
    dChart->setTitleBrush(titleColor);
    dChart->setTitleFont(QFont("", 11, QFont::Bold));
    dChart->legend()->hide();
    
    QChart *tChart = barView->chart();
    tChart->setBackgroundVisible(false);
    tChart->setTitleBrush(titleColor);
    tChart->setTitleFont(QFont("", 11, QFont::Bold));
    tChart->legend()->setLabelColor(text);
    tChart->setMargins(QMargins(5, 5, 5, 5));
    (void)sub; (void)grid;
}

void DashboardPage::onDonutHovered(QPieSlice *slice, bool state)
{
    if (slice) {
        slice->setExploded(state);
        if (state) {
            QToolTip::showText(QCursor::pos(), slice->property("tooltip").toString());
        } else {
            QToolTip::hideText();
        }
    }
}

void DashboardPage::updateCharts()
{
    ThemeManager &theme = ThemeManager::instance();
    setChartTheme();

    QChart *chart = donutView->chart();
    chart->removeAllSeries();

    int month = monthCb->currentData().toInt();
    int year = yearCb->currentData().toInt();
    QDate fromDate = DateUtils::firstDayOfMonth(year, month);
    QDate toDate = DateUtils::lastDayOfMonth(year, month);

    QVector<Transaction> allTx = AppContext::instance().transactionRepository().getAllTransactions();
    QList<Transaction> txList(allTx.begin(), allTx.end());
    txList = AppContext::instance().transactionService().filterByDateRange(txList, fromDate, toDate);

    QVector<Category> categories = AppContext::instance().categoryService().getAllCategories();
    QMap<int, QString> catNames;
    for (const Category &c : categories) catNames[c.getId()] = c.getName();

    QMap<int, double> expenseByCat;
    double totalExpense = 0;
    for (const Transaction &t : txList) {
        if (t.isExpense()) {
            expenseByCat[t.getCategoryId()] += t.getAmount();
            totalExpense += t.getAmount();
        }
    }

    QPieSeries *donut = new QPieSeries();
    donut->setHoleSize(0.5);
    donut->setPieSize(0.7);



    QList<int> ids = expenseByCat.keys();
    std::sort(ids.begin(), ids.end(), [&expenseByCat](int a, int b) { return expenseByCat.value(a) > expenseByCat.value(b); });

    // Xóa custom legend cũ
    QLayoutItem *child;
    while ((child = donutLegendLayout->takeAt(0)) != nullptr) {
        if (child->widget()) child->widget()->deleteLater();
        else if (child->layout()) {
            QLayoutItem *subchild;
            while ((subchild = child->layout()->takeAt(0)) != nullptr) {
                if (subchild->widget()) subchild->widget()->deleteLater();
                delete subchild;
            }
            delete child->layout();
        }
        delete child;
    }

    int colorIdx = 0;
    for (int catId : ids) {
        double value = expenseByCat.value(catId);
        if (value <= 0) continue;

        double pct = (totalExpense > 0) ? (value / totalExpense * 100.0) : 0;
        QString rawName = catNames.value(catId, "Không xác định");
        QString legendName = QString("%1 - %2 (%3%)").arg(rawName).arg(MoneyUtils::formatVND(value)).arg(QString::number(pct, 'f', 1));
        
        QColor sliceColor = StyleUtils::getCategoryColor(catId);
        
        // Hàng legendvào custom legend
        QHBoxLayout *row = new QHBoxLayout();
        row->setContentsMargins(0,0,0,0);
        
        QLabel *colorBox = new QLabel();
        colorBox->setFixedSize(12, 12);
        colorBox->setStyleSheet(QString("background-color: %1; border-radius: 2px;").arg(sliceColor.name()));
        
        QLabel *nameLbl = new QLabel(legendName);
        nameLbl->setStyleSheet("font-size: 13px; color: " + (theme.darkMode() ? QString("#E2E8F0") : QString("#334155")) + ";");
        
        row->addWidget(colorBox);
        row->addWidget(nameLbl);
        row->addStretch();
        
        QWidget *rowWidget = new QWidget();
        rowWidget->setLayout(row);
        donutLegendLayout->addWidget(rowWidget);
        
        // Thêm vào donut (ẩn label đi, chỉ hiện khi hover)
        QPieSlice *slice = donut->append(rawName, value);
        slice->setColor(sliceColor);
        slice->setLabelVisible(false);
        slice->setLabelColor(theme.textColor());
        slice->setLabelPosition(QPieSlice::LabelOutside);
        slice->setLabel(QString("%1: %2%").arg(rawName).arg(QString::number(pct, 'f', 1)));
        
        // Tooltip custom
        QString tooltipHtml = QString("<div style='background-color:#1E293B; color:white; padding:8px; border-radius:6px;'>"
                                      "<div style='font-weight:bold; font-size:14px; margin-bottom:4px;'>%1</div>"
                                      "<div style='color:#94A3B8;'>Số tiền: <b style='color:white;'>%2</b></div>"
                                      "<div style='color:#94A3B8;'>Tỉ lệ: <b style='color:white;'>%3%</b></div>"
                                      "</div>")
                                      .arg(rawName)
                                      .arg(MoneyUtils::formatVND(value))
                                      .arg(QString::number(pct, 'f', 1));
        slice->setProperty("tooltip", tooltipHtml);
        connect(slice, &QPieSlice::hovered, this, [this, slice](bool state) {
            onDonutHovered(slice, state);
        });
    }

    chart->addSeries(donut);
    donutCenterText->setHtml(QString("<div style='text-align:center;'><span style='color:%1; font-size:12px;'>Tổng Chi Tiêu</span><br/><span style='color:%2; font-size:16px; font-weight:bold;'>%3</span></div>")
                             .arg(theme.mutedColor().name()).arg(theme.textColor().name()).arg(MoneyUtils::formatVND(totalExpense)));
                             
    connect(chart, &QChart::geometryChanged, this, [chart, this]() {
        QRectF rect = chart->plotArea();
        QRectF textRect = donutCenterText->boundingRect();
        donutCenterText->setPos(rect.center().x() - textRect.width() / 2, rect.center().y() - textRect.height() / 2);
    });
}

void DashboardPage::updateTrendChart()
{
    ThemeManager &theme = ThemeManager::instance();
    QChart *chart = barView->chart();
    chart->removeAllSeries();
    for (auto axis : chart->axes()) chart->removeAxis(axis);

    int month = monthCb->currentData().toInt();
    int year = yearCb->currentData().toInt();
    QVector<Transaction> allTx = AppContext::instance().transactionRepository().getAllTransactions();
    QList<Transaction> txList(allTx.begin(), allTx.end());

    QLineSeries *incomeSeries = new QLineSeries();
    incomeSeries->setName("Thu Nhập");
    QPen incPen(QColor("#059669")); incPen.setWidth(3);
    incomeSeries->setPen(incPen);
    
    QLineSeries *expenseSeries = new QLineSeries();
    expenseSeries->setName("Chi Tiêu");
    QPen expPen(QColor("#E11D48")); expPen.setWidth(3);
    expenseSeries->setPen(expPen);

    QStringList categories;
    double maxVal = 0;

    for (int i = 5; i >= 0; --i) {
        int m = month - i;
        int y = year;
        while (m <= 0) { m += 12; y--; }
        QDate fromDate = DateUtils::firstDayOfMonth(y, m);
        QDate toDate = DateUtils::lastDayOfMonth(y, m);
        Report report = AppContext::instance().reportService().generateReport(txList, fromDate, toDate);
        
        categories << QString("%1/%2").arg(m).arg(y % 100);
        incomeSeries->append(5 - i, report.getTotalIncome());
        expenseSeries->append(5 - i, report.getTotalExpense());
        if (report.getTotalIncome() > maxVal) maxVal = report.getTotalIncome();
        if (report.getTotalExpense() > maxVal) maxVal = report.getTotalExpense();
    }

    chart->addSeries(incomeSeries);
    chart->addSeries(expenseSeries);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    axisX->setLabelsColor(theme.bodyTextColor());
    chart->addAxis(axisX, Qt::AlignBottom);
    incomeSeries->attachAxis(axisX);
    expenseSeries->attachAxis(axisX);

    QCategoryAxis *axisY = new QCategoryAxis();
    axisY->setLabelsColor(theme.bodyTextColor());
    double maxY = maxVal * 1.1;
    if (maxY <= 0) maxY = 1000;
    
    int tickCount = 5;
    double step = maxY / tickCount;
    for (int i = 0; i <= tickCount; ++i) {
        double val = step * i;
        // Padding spaces to ensure unique labels
        QString label = MoneyUtils::formatCompact(val) + QString(" ").repeated(i);
        axisY->append(label, val);
    }
    axisY->setRange(0, maxY);
    chart->addAxis(axisY, Qt::AlignLeft);
    incomeSeries->attachAxis(axisY);
    expenseSeries->attachAxis(axisY);

    auto hoverHandler = [this, categories](const QPointF &point, bool state, const QString &type) {
        if (state) {
            int idx = qRound(point.x());
            QString monthStr = (idx >= 0 && idx < categories.size()) ? categories.at(idx) : "";
            QString tooltip = QString("Tháng %1\n%2: %3")
                                  .arg(monthStr)
                                  .arg(type)
                                  .arg(MoneyUtils::formatVND(point.y()));
            QToolTip::showText(QCursor::pos(), tooltip);
        } else {
            QToolTip::hideText();
        }
    };

    connect(incomeSeries, &QLineSeries::hovered, this, [hoverHandler](const QPointF &p, bool state) {
        hoverHandler(p, state, "Thu Nhập");
    });
    connect(expenseSeries, &QLineSeries::hovered, this, [hoverHandler](const QPointF &p, bool state) {
        hoverHandler(p, state, "Chi Tiêu");
    });
}

void DashboardPage::exportCsv()
{
    int month = monthCb->currentData().toInt();
    int year = yearCb->currentData().toInt();
    QDate fromDate = DateUtils::firstDayOfMonth(year, month);
    QDate toDate = DateUtils::lastDayOfMonth(year, month);

    QString defaultName = QString("dashboard_%1_%2.csv").arg(year).arg(month);
    QString fileName = QFileDialog::getSaveFileName(this, "Xuất CSV", defaultName, "CSV files (*.csv)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Lỗi", "Không thể ghi vào file đã chọn!");
        return;
    }

    QVector<Transaction> allTx = AppContext::instance().transactionRepository().getAllTransactions();
    QList<Transaction> txList(allTx.begin(), allTx.end());
    QList<Transaction> monthTx = AppContext::instance().transactionService().filterByDateRange(txList, fromDate, toDate);

    QVector<Category> categories = AppContext::instance().categoryService().getAllCategories();
    QMap<int, QString> catNames;
    for (const Category &c : categories) catNames[c.getId()] = c.getName();

    QVector<Account> accounts = AppContext::instance().accountService().getAllAccounts();
    QMap<int, QString> accountNames;
    for (const Account &a : accounts) accountNames[a.getId()] = a.getName();

    QTextStream out(&file);
    out << "Ngày,Tài Khoản,Danh Mục,Loại,Số Tiền,Ghi Chú\n";
    for (const Transaction &t : monthTx) {
        out << t.getDate().toString("dd/MM/yyyy") << ","
            << accountNames.value(t.getAccountId(), "") << ","
            << catNames.value(t.getCategoryId(), "") << ","
            << (t.isIncome() ? "Thu Nhập" : "Chi Tiêu") << ","
            << QString::number(t.getAmount(), 'f', 2) << ","
            << "\"" << t.getNote() << "\"\n";
    }

    Report report = AppContext::instance().reportService().generateReport(txList, fromDate, toDate);
    out << "\nTổng kết,,,Tổng Thu Nhập,," << QString::number(report.getTotalIncome(), 'f', 2) << "\n";
    out << "Tổng kết,,,Tổng Chi Tiêu,," << QString::number(report.getTotalExpense(), 'f', 2) << "\n";
    out << "Tổng kết,,,Số Dư,," << QString::number(report.getRemainingBalance(), 'f', 2) << "\n";

    file.close();
    QMessageBox::information(this, "Xuất File", QString("Tổng quan đã được xuất ra:\n%1").arg(fileName));
}
