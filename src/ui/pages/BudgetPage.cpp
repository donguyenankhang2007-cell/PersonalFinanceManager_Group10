// ============================================
// File: src/ui/pages/BudgetPage.cpp
// Mo ta: Trang ngan sach — card tien do chi tieu theo
//        tung danh muc (progress bar), Edit/Delete icon moi hang.
// ============================================
#include "BudgetPage.h"
#include "../dialogs/BudgetDialog.h"
#include "../RowActions.h"
#include "../theme/ThemeManager.h"
#include "../../app/AppContext.h"
#include "../../utils/MoneyUtils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>

#include "../../utils/StyleUtils.h"
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QScrollArea>
#include <QDate>
#include <QMap>
#include <QGraphicsDropShadowEffect>
#include <QMessageBox>
#include <QComboBox>

BudgetPage::BudgetPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(15);

    // Tiêu đề
    QHBoxLayout *header = new QHBoxLayout();
    QVBoxLayout *titleBox = new QVBoxLayout();
    titleBox->setSpacing(2);
    titleLabel = new QLabel("Ngân Sách");
    titleLabel->setObjectName("pageTitle");
    subtitleLabel = new QLabel("Theo dõi giới hạn chi tiêu hàng tháng");
    subtitleLabel->setObjectName("pageSubtitle");
    titleBox->addWidget(titleLabel);
    titleBox->addWidget(subtitleLabel);

    btnAdd = new QPushButton("+ Thêm Ngân Sách");
    btnAdd->setObjectName("primaryBtn");
    btnAdd->setCursor(Qt::PointingHandCursor);
    header->addLayout(titleBox);
    header->addStretch();
    header->addWidget(btnAdd);
    mainLayout->addLayout(header);

    // === Filter & Sort ===
    QHBoxLayout *filterRow = new QHBoxLayout();
    filterRow->setSpacing(10);
    
    monthFilterCb = new QComboBox();
    monthFilterCb->addItem("Tất cả các tháng", "");
    // Dữ liệu tháng sẽ được populate trong loadBudgets() lần đầu tiên nếu muốn
    
    sortCb = new QComboBox();
    sortCb->addItem("Xếp theo Ngày (Mới nhất)", "date_desc");
    sortCb->addItem("Xếp theo Ngày (Cũ nhất)", "date_asc");
    sortCb->addItem("Xếp theo % Sử dụng (Cao đến thấp)", "usage_desc");
    sortCb->addItem("Xếp theo Danh mục", "category");
    
    filterRow->addWidget(new QLabel("Lọc:"));
    filterRow->addWidget(monthFilterCb);
    filterRow->addSpacing(20);
    filterRow->addWidget(new QLabel("Sắp xếp:"));
    filterRow->addWidget(sortCb);
    filterRow->addStretch();
    mainLayout->addLayout(filterRow);

    // === Danh sách budget ===
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);

    budgetListWidget = new QWidget();
    budgetListWidget->setObjectName("card");
    budgetListLayout = new QVBoxLayout(budgetListWidget);
    budgetListLayout->setSpacing(14);
    budgetListLayout->setContentsMargins(16, 16, 16, 16);
    budgetListLayout->addStretch();

    scrollArea->setWidget(budgetListWidget);

    mainLayout->addWidget(scrollArea);

    setLayout(mainLayout);

    // Kết nối
    connect(btnAdd, &QPushButton::clicked, this, &BudgetPage::onAddBudget);

    // Tự cập nhật khi giao dịch thay đổi (số đã chi thay đổi)
    TransactionService &txService = AppContext::instance().transactionService();
    connect(&txService, &TransactionService::transactionAdded,
            this, [this](const Transaction &) { loadBudgets(); });
    connect(&txService, &TransactionService::transactionUpdated,
            this, [this](const Transaction &) { loadBudgets(); });
    connect(&txService, &TransactionService::transactionRemoved,
            this, [this](int) { loadBudgets(); });

    connect(monthFilterCb, &QComboBox::currentIndexChanged, this, &BudgetPage::onFilterChanged);
    connect(sortCb, &QComboBox::currentIndexChanged, this, &BudgetPage::onFilterChanged);

    loadBudgets();
}

void BudgetPage::onFilterChanged()
{
    loadBudgets();
}



void BudgetPage::loadBudgets()
{
    // Clear danh sách cũ
    QLayoutItem *child;
    while ((child = budgetListLayout->takeAt(0)) != nullptr) {
        if (child->widget()) {
            delete child->widget();
        }
        delete child;
    }

    BudgetService &budgetService = AppContext::instance().budgetService();
    QVector<Budget> allBudgets = budgetService.getAllBudgets();

    // Điền bộ lọc tháng nếu trống
    if (monthFilterCb->count() <= 1 && !allBudgets.isEmpty()) {
        QSet<QString> months;
        for (const Budget &b : allBudgets) {
            months.insert(QString("%1/%2").arg(b.getMonth(), 2, 10, QChar('0')).arg(b.getYear()));
        }
        QList<QString> sortedMonths = months.values();
        std::sort(sortedMonths.begin(), sortedMonths.end(), std::greater<QString>());
        for (const QString &m : sortedMonths) {
            monthFilterCb->addItem(m, m);
        }
    }

    // Lọc
    QString selectedMonth = monthFilterCb->currentData().toString();
    QVector<Budget> budgets;
    for (const Budget &b : allBudgets) {
        if (selectedMonth.isEmpty() || selectedMonth == QString("%1/%2").arg(b.getMonth(), 2, 10, QChar('0')).arg(b.getYear())) {
            budgets.append(b);
        }
    }

    // Sắp xếp
    QString sortMode = sortCb->currentData().toString();
    QVector<Transaction> allTx = AppContext::instance().transactionService().getAllTransactions();
    QList<Transaction> txList(allTx.begin(), allTx.end());
    
    QVector<Category> categories = AppContext::instance().categoryService().getAllCategories();
    QMap<int, QString> categoryNames;
    for (const Category &cat : categories) {
        categoryNames[cat.getId()] = cat.getName();
    }

    std::sort(budgets.begin(), budgets.end(), [&](const Budget &a, const Budget &b) {
        if (sortMode == "date_asc") {
            if (a.getYear() != b.getYear()) return a.getYear() < b.getYear();
            return a.getMonth() < b.getMonth();
        } else if (sortMode == "usage_desc") {
            double u1 = budgetService.getUsagePercent(a, txList);
            double u2 = budgetService.getUsagePercent(b, txList);
            return u1 > u2;
        } else if (sortMode == "category") {
            return categoryNames.value(a.getCategoryId()) < categoryNames.value(b.getCategoryId());
        } else {
            // Default: date_desc
            if (a.getYear() != b.getYear()) return a.getYear() > b.getYear();
            return a.getMonth() > b.getMonth();
        }
    });

    if (budgets.isEmpty()) {
        QLabel *emptyLabel = new QLabel("📭\nChưa có ngân sách nào.\nNhấn '+ Thêm Ngân Sách' để bắt đầu.");
        emptyLabel->setAlignment(Qt::AlignCenter);
        emptyLabel->setObjectName("emptyLabel");
        budgetListLayout->addWidget(emptyLabel);
        budgetListLayout->addStretch();
        return;
    }

    QString currentHeader = "";

    for (const Budget &b : budgets) {
        // Grouping by Month/Year
        QString headerText = QString("Tháng %1 / %2").arg(b.getMonth()).arg(b.getYear());
        if (headerText != currentHeader && sortMode != "category" && sortMode != "usage_desc") {
            currentHeader = headerText;
            QLabel *headerLabel = new QLabel(currentHeader);
            headerLabel->setObjectName("budgetHeader");
            budgetListLayout->addWidget(headerLabel);
        }

        double spent = budgetService.getSpentAmount(b, txList);
        double limit = b.getAmount();
        double usage = budgetService.getUsagePercent(b, txList);
        int percent = qMin(100, (int)(usage * 100));

        QWidget *card = new QWidget();
        card->setObjectName("card");
        
        QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
        shadow->setBlurRadius(20);
        shadow->setColor(QColor(0, 0, 0, 15));
        shadow->setOffset(0, 4);
        card->setGraphicsEffect(shadow);

        QVBoxLayout *cardLayout = new QVBoxLayout(card);
        cardLayout->setContentsMargins(16, 12, 16, 12);
        cardLayout->setSpacing(8);

        QString rawCatName = categoryNames.value(b.getCategoryId(), "Không xác định");
        QString catName = rawCatName;
        
        // Add warning icon if > 150%
        if (usage > 1.5) {
            catName = "⚠️ " + rawCatName;
        }

        // Dòng 1: tên danh mục + tháng/năm + hành động
        QHBoxLayout *topRow = new QHBoxLayout();
        topRow->setAlignment(Qt::AlignVCenter);
        
        QLabel *nameLabel = new QLabel(catName);
        nameLabel->setObjectName("budgetName");
        nameLabel->setWordWrap(true); // Responsive wrap
        
        QLabel *periodLabel = new QLabel(QString("%1 / %2").arg(b.getMonth()).arg(b.getYear()));
        periodLabel->setObjectName("cardTitle");
        
        topRow->addWidget(nameLabel, 1);
        topRow->addWidget(periodLabel);
        topRow->addSpacing(12);
        topRow->addWidget(RowActions::create(b.getId(),
            [this](int id) { editBudgetById(id); },
            [this](int id) { deleteBudgetById(id); },
            this));
            
        cardLayout->addLayout(topRow);

        // Dòng 2: số đã chi / hạn mức + % + badge
        QHBoxLayout *valueRow = new QHBoxLayout();
        valueRow->setAlignment(Qt::AlignVCenter);
        
        QLabel *valueLabel = new QLabel(
            QString("%1 / %2  (%3%)")
                .arg(MoneyUtils::formatVND(spent))
                .arg(MoneyUtils::formatVND(limit))
                .arg(QString::number(usage * 100, 'f', 0)));
        valueLabel->setObjectName("cardTitle");
        valueRow->addWidget(valueLabel);
        valueRow->addStretch();
        
        QLabel *badgeLabel = new QLabel();
        badgeLabel->setAlignment(Qt::AlignCenter);
        QString badgeStyle = "padding: 2px 8px; border-radius: 10px; font-size: 11px; font-weight: bold; ";
        if (usage >= 1.0) {
            badgeLabel->setText("Vượt ngân sách");
            badgeLabel->setStyleSheet(badgeStyle + "background-color: #FEE2E2; color: #EF4444;");
        } else if (usage >= 0.8) {
            badgeLabel->setText("Sắp hết");
            badgeLabel->setStyleSheet(badgeStyle + "background-color: #FEF3C7; color: #F59E0B;");
        } else {
            badgeLabel->setText("Còn dư");
            badgeLabel->setStyleSheet(badgeStyle + "background-color: #D1FAE5; color: #10B981;");
        }
        valueRow->addWidget(badgeLabel);
        
        cardLayout->addLayout(valueRow);

        // Dòng 3: progress bar
        QProgressBar *bar = new QProgressBar();
        bar->setFixedHeight(12);
        bar->setTextVisible(false);
        bar->setRange(0, 100);
        
        int percentInt = (int)percent;
        if (percentInt > 100) percentInt = 100;
        bar->setValue(percentInt);

        bar->setStyleSheet(StyleUtils::getBudgetProgressStyle(percent));
        cardLayout->addWidget(bar);

        budgetListLayout->addWidget(card);
    }

    budgetListLayout->addStretch();
}

void BudgetPage::onAddBudget()
{
    QVector<Category> expenseCategories;
    QVector<Category> categories =
        AppContext::instance().categoryService().getAllCategories();
    for (const Category &cat : categories) {
        if (cat.getType() == CategoryType::Expense)
            expenseCategories.append(cat);
    }

    if (expenseCategories.isEmpty()) {
        QMessageBox::warning(this, "Lỗi",
                             "Vui lòng thêm danh mục chi tiêu trước!");
        return;
    }

    BudgetDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QString errorMessage;
        if (!AppContext::instance()
                 .budgetService()
                 .addBudget(dialog.getBudget(), &errorMessage)) {
            QMessageBox::warning(this, "Lỗi", errorMessage);
        }
        // Danh sách tự cập nhật qua signal budgetAdded
    }
}

void BudgetPage::editBudgetById(int id)
{
    Budget budget = AppContext::instance().budgetRepository().getBudgetById(id);

    if (budget.getId() <= 0) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy ngân sách!");
        return;
    }

    BudgetDialog dialog(budget, this);

    if (dialog.exec() == QDialog::Accepted) {
        QString errorMessage;
        if (!AppContext::instance()
                 .budgetService()
                 .updateBudget(dialog.getBudget(), &errorMessage)) {
            QMessageBox::warning(this, "Lỗi", errorMessage);
        }
        // Danh sách tự cập nhật qua signal budgetUpdated
    }
}

void BudgetPage::deleteBudgetById(int id)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Xác nhận",
        "Bạn có chắc chắn muốn xóa ngân sách này không?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QString errorMessage;
        if (!AppContext::instance()
                 .budgetService()
                 .removeBudget(id, &errorMessage)) {
            QMessageBox::warning(this, "Lỗi", errorMessage);
        }
        // Danh sách tự cập nhật qua signal budgetRemoved
    }
}
