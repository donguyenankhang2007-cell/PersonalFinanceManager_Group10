// ============================================
// File: src/ui/pages/BudgetPage.cpp
// Nguoi viet: Viet Tuong (GUI Developer)
// Mo ta: Trang ngan sach — hien thi tien do
//        chi tieu theo tung danh muc bang progress bar
//        Ket noi BudgetRepository, BudgetService
// ============================================
#include "BudgetPage.h"
#include "../../app/AppContext.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QProgressBar>
#include <QGroupBox>
#include <QMessageBox>
#include <QScrollArea>
#include <QDate>
#include <QMap>

BudgetPage::BudgetPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(15);

    // Tiêu đề
    QLabel *title = new QLabel("Budget Management");
    title->setStyleSheet(
        "font-size: 22px;"
        "font-weight: bold;"
        "color: #1e272e;"
        "padding-bottom: 10px;");

    // === Form thêm budget ===
    QGroupBox *formGroup = new QGroupBox("Add New Budget");
    formGroup->setStyleSheet(
        "QGroupBox { font-weight: bold; font-size: 14px; "
        "color: #1e272e; background-color: #ffffff; "
        "border: 1px solid #dcdde1; border-radius: 8px; "
        "padding-top: 20px; margin-top: 10px; }"
        "QGroupBox::title { subcontrol-origin: margin; "
        "left: 15px; padding: 0 5px; }");

    QFormLayout *formLayout = new QFormLayout(formGroup);

    categoryCombo = new QComboBox();
    amountSpin = new QDoubleSpinBox();
    amountSpin->setRange(1, 999999999);
    amountSpin->setDecimals(0);
    amountSpin->setSuffix(" VND");

    monthSpin = new QSpinBox();
    monthSpin->setRange(1, 12);
    monthSpin->setValue(QDate::currentDate().month());

    yearSpin = new QSpinBox();
    yearSpin->setRange(2020, 2030);
    yearSpin->setValue(QDate::currentDate().year());

    btnAdd = new QPushButton("+ Add Budget");

    formLayout->addRow("Category:", categoryCombo);
    formLayout->addRow("Limit Amount:", amountSpin);
    formLayout->addRow("Month:", monthSpin);
    formLayout->addRow("Year:", yearSpin);
    formLayout->addRow("", btnAdd);

    // === Danh sách budget ===
    QScrollArea *scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setStyleSheet(
        "QScrollArea { border: 1px solid #E0E0E0; border-radius: 8px;"
        "  background-color: #F5F5F5; }");

    budgetListWidget = new QWidget();
    budgetListWidget->setStyleSheet("background-color: #F5F5F5;");
    budgetListLayout = new QVBoxLayout(budgetListWidget);
    budgetListLayout->setSpacing(10);
    budgetListLayout->setContentsMargins(10, 10, 10, 10);
    budgetListLayout->addStretch();

    scrollArea->setWidget(budgetListWidget);

    mainLayout->addWidget(title);
    mainLayout->addWidget(formGroup);
    mainLayout->addWidget(scrollArea);

    setLayout(mainLayout);

    // Kết nối
    connect(btnAdd, &QPushButton::clicked,
            this, &BudgetPage::onAddBudget);

    // Load dữ liệu
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

    // Load categories for combo box
    categoryCombo->clear();
    QVector<Category> categories =
        AppContext::instance().categoryRepository().getAllCategories();
    QMap<int, QString> categoryNames;
    for (const Category &cat : categories) {
        categoryCombo->addItem(cat.getName(), cat.getId());
        categoryNames[cat.getId()] = cat.getName();
    }

    // Load budgets
    QVector<Budget> budgets =
        AppContext::instance().budgetRepository().getAllBudgets();

    // Load transactions for calculating spent
    QVector<Transaction> allTransactions =
        AppContext::instance().transactionRepository().getAllTransactions();

    for (const Budget &b : budgets) {
        // Tính tổng chi tiêu thực tế
        double spent = 0;
        for (const Transaction &t : allTransactions) {
            if (t.getCategoryId() == b.getCategoryId()
                && t.getType() == "expense"
                && t.getDate().month() == b.getMonth()
                && t.getDate().year() == b.getYear()) {
                spent += t.getAmount();
            }
        }

        int percent = (b.getAmount() > 0)
                          ? qMin(100, (int)(spent / b.getAmount() * 100))
                          : 0;

        // Card cho mỗi budget
        QWidget *card = new QWidget();
        card->setStyleSheet(
            "background-color: #ffffff;"
            "border: 1px solid #dcdde1;"
            "border-radius: 8px;"
            "padding: 12px;");

        QVBoxLayout *cardLayout = new QVBoxLayout(card);

        QString catName = categoryNames.value(b.getCategoryId(), "Unknown");

        QLabel *label = new QLabel(
            QString("%1  —  %2 / %3 VND  (%4/%5)")
                .arg(catName)
                .arg(QString::number(spent, 'f', 0))
                .arg(QString::number(b.getAmount(), 'f', 0))
                .arg(b.getMonth())
                .arg(b.getYear()));
        label->setStyleSheet(
            "font-size: 13px; color: #2f3640; font-weight: bold;"
            "border: none;");

        QProgressBar *bar = new QProgressBar();
        bar->setRange(0, 100);
        bar->setValue(percent);

        if (percent >= 90) {
            bar->setStyleSheet(
                "QProgressBar { background-color: #dcdde1; border: none; "
                "border-radius: 8px; height: 18px; text-align: center; }"
                "QProgressBar::chunk { background-color: #ff3f34; "
                "border-radius: 8px; }");
        } else if (percent >= 70) {
            bar->setStyleSheet(
                "QProgressBar { background-color: #dcdde1; border: none; "
                "border-radius: 8px; height: 18px; text-align: center; }"
                "QProgressBar::chunk { background-color: #ffa801; "
                "border-radius: 8px; }");
        }

        QPushButton *btnDel = new QPushButton("Delete");
        btnDel->setStyleSheet(
            "background-color: #ff3f34; color: white;"
            "border: none; padding: 5px 12px;"
            "font-size: 11px; border-radius: 4px;");
        btnDel->setFixedWidth(70);
        btnDel->setProperty("budgetId", b.getId());

        connect(btnDel, &QPushButton::clicked,
                this, &BudgetPage::onDeleteBudget);

        cardLayout->addWidget(label);
        cardLayout->addWidget(bar);
        cardLayout->addWidget(btnDel);

        budgetListLayout->insertWidget(
            budgetListLayout->count() - 1, card);
    }

    // Thêm stretch ở cuối
    budgetListLayout->addStretch();
}

void BudgetPage::onAddBudget()
{
    if (categoryCombo->count() == 0) {
        QMessageBox::warning(this, "Error",
                             "Please add categories first!");
        return;
    }

    int categoryId = categoryCombo->currentData().toInt();
    double amount = amountSpin->value();
    int month = monthSpin->value();
    int year = yearSpin->value();

    if (amount <= 0) {
        QMessageBox::warning(this, "Error",
                             "Budget amount must be greater than 0!");
        return;
    }

    Budget budget(0, categoryId, amount, month, year);

    if (AppContext::instance().budgetRepository().addBudget(budget)) {
        loadBudgets();
    } else {
        QMessageBox::warning(this, "Error",
                             "Failed to add budget!");
    }
}

void BudgetPage::onDeleteBudget()
{
    QPushButton *btn = qobject_cast<QPushButton *>(sender());
    if (!btn) return;

    int id = btn->property("budgetId").toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm",
        "Are you sure you want to delete this budget?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (AppContext::instance().budgetRepository().deleteBudget(id)) {
            loadBudgets();
        }
    }
}