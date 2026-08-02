// ============================================
// File: src/ui/pages/TransactionPage.cpp
// Mo ta: Lich su giao dich dang bang + thanh loc
//        (loai, account, category, khoang thoi gian, tim ghi chu).
//        Moi hang co icon Edit/Delete, double-click de sua nhanh.
// ============================================
#include "TransactionPage.h"
#include "../dialogs/TransactionDialog.h"
#include "../RowActions.h"
#include "../theme/ThemeManager.h"
#include "../../app/AppContext.h"
#include "../../utils/MoneyUtils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QComboBox>
#include <QLineEdit>
#include <QDateEdit>
#include <QCheckBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QMap>
#include <QColor>

TransactionPage::TransactionPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(15);

    // Tiêu đề
    QHBoxLayout *header = new QHBoxLayout();
    QVBoxLayout *titleBox = new QVBoxLayout();
    titleBox->setSpacing(2);
    QLabel *title = new QLabel("Giao Dịch");
    title->setObjectName("pageTitle");
    QLabel *subtitle = new QLabel("Lịch sử giao dịch chi tiết");
    subtitle->setObjectName("pageSubtitle");
    titleBox->addWidget(title);
    titleBox->addWidget(subtitle);

    btnAdd = new QPushButton("+ Thêm Giao Dịch");
    btnAdd->setObjectName("primaryBtn");
    btnAdd->setCursor(Qt::PointingHandCursor);
    header->addLayout(titleBox);
    header->addStretch();
    header->addWidget(btnAdd);
    layout->addLayout(header);

    // === Thanh lọc ===
    QWidget *filterBar = new QWidget();
    filterBar->setObjectName("card");
    QHBoxLayout *filterLayout = new QHBoxLayout(filterBar);
    filterLayout->setContentsMargins(12, 10, 12, 10);
    filterLayout->setSpacing(8);

    filterType = new QComboBox();
    filterType->addItems({"Tất cả loại", "Thu Nhập", "Chi Tiêu"});

    filterAccount = new QComboBox();
    filterAccount->addItem("Tất cả tài khoản");

    filterCategory = new QComboBox();
    filterCategory->addItem("Tất cả danh mục");

    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Tìm ghi chú...");
    searchEdit->setClearButtonEnabled(true);

    periodCheck = new QCheckBox("Lọc ngày");
    periodCheck->setToolTip("Bật/tắt lọc theo khoảng thời gian");
    periodCheck->setCursor(Qt::PointingHandCursor);

    QFrame *dateRangeFrame = new QFrame();
    dateRangeFrame->setObjectName("dateRangeFrame");
    QHBoxLayout *dateLayout = new QHBoxLayout(dateRangeFrame);
    dateLayout->setContentsMargins(8, 0, 8, 0);
    dateLayout->setSpacing(4);

    fromDate = new QDateEdit();
    fromDate->setCalendarPopup(true);
    fromDate->setDisplayFormat("dd/MM/yyyy");
    fromDate->setDate(QDate::currentDate().addMonths(-1));
    fromDate->setObjectName("dateRangeInput");
    
    QLabel *dash = new QLabel("-");
    
    toDate = new QDateEdit();
    toDate->setCalendarPopup(true);
    toDate->setDisplayFormat("dd/MM/yyyy");
    toDate->setDate(QDate::currentDate());
    toDate->setObjectName("dateRangeInput");

    dateLayout->addWidget(fromDate);
    dateLayout->addWidget(dash);
    dateLayout->addWidget(toDate);

    btnClear = new QPushButton("Xóa bộ lọc");
    btnClear->setObjectName("ghostBtn");
    btnClear->setCursor(Qt::PointingHandCursor);

    filterLayout->addWidget(filterType);
    filterLayout->addWidget(filterAccount);
    filterLayout->addWidget(filterCategory);
    filterLayout->addWidget(searchEdit, 1);
    filterLayout->addWidget(periodCheck);
    filterLayout->addWidget(dateRangeFrame);
    filterLayout->addWidget(btnClear);
    layout->addWidget(filterBar);

    // Bảng giao dịch
    table = new QTableWidget();
    table->setColumnCount(8);
    table->setHorizontalHeaderLabels(
        {"ID", "Ngày", "Tài Khoản", "Danh Mục", "Số Tiền (VND)",
         "Loại", "Ghi Chú", "Hành Động"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Fixed);
    table->setColumnWidth(7, 92);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setVisible(false);

    layout->addWidget(table);

    setLayout(layout);

    // Kết nối signals
    connect(btnAdd, &QPushButton::clicked, this, &TransactionPage::onAddTransaction);
    connect(btnClear, &QPushButton::clicked, this, &TransactionPage::clearFilters);
    connect(filterType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TransactionPage::applyFilters);
    connect(filterAccount, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TransactionPage::applyFilters);
    connect(filterCategory, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TransactionPage::applyFilters);
    connect(searchEdit, &QLineEdit::textChanged,
            this, &TransactionPage::applyFilters);
    connect(periodCheck, &QCheckBox::toggled,
            this, &TransactionPage::applyFilters);
    connect(fromDate, &QDateEdit::dateChanged,
            this, &TransactionPage::applyFilters);
    connect(toDate, &QDateEdit::dateChanged,
            this, &TransactionPage::applyFilters);

    // Double-click hàng → sửa nhanh
    connect(table, &QTableWidget::cellDoubleClicked,
            this, [this](int row, int) {
                if (row >= 0) {
                    int id = table->item(row, 0)->text().toInt();
                    editTransactionById(id);
                }
            });

    // Tự cập nhật bảng khi service phát signal (Observer)
    TransactionService &txService = AppContext::instance().transactionService();
    connect(&txService, &TransactionService::transactionAdded,
            this, &TransactionPage::loadTransactions);
    connect(&txService, &TransactionService::transactionUpdated,
            this, &TransactionPage::loadTransactions);
    connect(&txService, &TransactionService::transactionRemoved,
            this, &TransactionPage::loadTransactions);

    // Đổi mật độ bảng → áp chiều cao hàng
    connect(&ThemeManager::instance(), &ThemeManager::densityChanged,
            this, &TransactionPage::loadTransactions);

    loadTransactions();
}

void TransactionPage::clearFilters()
{
    filterType->setCurrentIndex(0);
    filterAccount->setCurrentIndex(0);
    filterCategory->setCurrentIndex(0);
    searchEdit->clear();
    periodCheck->setChecked(false);
    applyFilters();
}

void TransactionPage::applyFilters()
{
    loadTransactions();
}

void TransactionPage::loadTransactions()
{
    table->setRowCount(0);

    QVector<Transaction> transactions =
        AppContext::instance().transactionService().getAllTransactions();

    // Tải tên danh mục để hiển thị
    QVector<Category> categories =
        AppContext::instance().categoryService().getAllCategories();
    QMap<int, QString> categoryNames;
    for (const Category &c : categories) {
        categoryNames[c.getId()] = c.getName();
    }

    // Tải tên tài khoản để hiển thị
    QVector<Account> accounts =
        AppContext::instance().accountService().getAllAccounts();
    QMap<int, QString> accountNames;
    for (const Account &a : accounts) {
        accountNames[a.getId()] = a.getName();
    }

    // Cập nhật danh sách filter (giữ lựa chọn hiện tại)
    QString selAccount = filterAccount->currentText();
    QString selCategory = filterCategory->currentText();
    filterAccount->blockSignals(true);
    filterAccount->clear();
    filterAccount->addItem("Tất cả tài khoản");
    for (const Account &a : accounts) {
        filterAccount->addItem(a.getName(), a.getId());
    }
    int accIdx = filterAccount->findText(selAccount);
    filterAccount->setCurrentIndex(accIdx < 0 ? 0 : accIdx);
    filterAccount->blockSignals(false);

    filterCategory->blockSignals(true);
    filterCategory->clear();
    filterCategory->addItem("Tất cả danh mục");
    for (const Category &c : categories) {
        filterCategory->addItem(c.getName(), c.getId());
    }
    int catIdx = filterCategory->findText(selCategory);
    filterCategory->setCurrentIndex(catIdx < 0 ? 0 : catIdx);
    filterCategory->blockSignals(false);

    // === Áp dụng bộ lọc ===
    QList<Transaction> list(transactions.begin(), transactions.end());
    TransactionService &txService = AppContext::instance().transactionService();

    int typeIdx = filterType->currentIndex();
    if (typeIdx == 1) {
        QList<Transaction> tmp;
        for (const Transaction &t : list)
            if (t.isIncome()) tmp.append(t);
        list = tmp;
    } else if (typeIdx == 2) {
        QList<Transaction> tmp;
        for (const Transaction &t : list)
            if (t.isExpense()) tmp.append(t);
        list = tmp;
    }

    int accountId = filterAccount->currentData().toInt();
    if (accountId > 0)
        list = txService.filterByAccount(list, accountId);

    int categoryId = filterCategory->currentData().toInt();
    if (categoryId > 0)
        list = txService.filterByCategory(list, categoryId);

    if (periodCheck->isChecked() && fromDate->date() <= toDate->date()) {
        list = txService.filterByDateRange(list, fromDate->date(), toDate->date());
    }

    QString keyword = searchEdit->text().trimmed();
    if (!keyword.isEmpty()) {
        QList<Transaction> tmp;
        for (const Transaction &t : list) {
            QString account = accountNames.value(t.getAccountId(), "");
            QString category = categoryNames.value(t.getCategoryId(), "");
            if (t.getNote().contains(keyword, Qt::CaseInsensitive) ||
                account.contains(keyword, Qt::CaseInsensitive) ||
                category.contains(keyword, Qt::CaseInsensitive)) {
                tmp.append(t);
            }
        }
        list = tmp;
    }

    int rowHeight = ThemeManager::instance().tableRowHeight();

    for (const Transaction &t : list) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setRowHeight(row, rowHeight);

        table->setItem(row, 0,
                       new QTableWidgetItem(QString::number(t.getId())));
        table->setItem(row, 1,
                       new QTableWidgetItem(
                           t.getDate().toString("dd/MM/yyyy")));
        table->setItem(row, 2,
                       new QTableWidgetItem(
                           accountNames.value(t.getAccountId(), "Không xác định")));
        table->setItem(row, 3,
                       new QTableWidgetItem(
                           categoryNames.value(t.getCategoryId(), "Không xác định")));

        QTableWidgetItem *amountItem = new QTableWidgetItem(
            MoneyUtils::formatVND(t.getAmount()));
        amountItem->setForeground(t.isIncome()
                                      ? QColor("#059669")
                                      : QColor("#E11D48"));
        table->setItem(row, 4, amountItem);

        QTableWidgetItem *typeItem = new QTableWidgetItem(
            t.isIncome() ? "Thu Nhập" : "Chi Tiêu");
        typeItem->setForeground(t.isIncome()
                                    ? QColor("#059669")
                                    : QColor("#E11D48"));
        table->setItem(row, 5, typeItem);

        table->setItem(row, 6, new QTableWidgetItem(t.getNote()));

        table->setCellWidget(row, 7,
            RowActions::create(t.getId(),
                [this](int id) { editTransactionById(id); },
                [this](int id) { deleteTransactionById(id); },
                this));
    }
}

void TransactionPage::onAddTransaction()
{
    TransactionDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        Transaction t = dialog.getTransaction();

        QString errorMessage;
        if (!AppContext::instance()
                 .transactionService()
                 .addTransaction(t, &errorMessage)) {
            QMessageBox::warning(this, "Lỗi", errorMessage);
        }
        // Bảng tự cập nhật qua signal transactionAdded
    }
}

void TransactionPage::editTransactionById(int id)
{
    Transaction t =
        AppContext::instance().transactionService().getTransactionById(id);

    TransactionDialog dialog(t, this);

    if (dialog.exec() == QDialog::Accepted) {
        Transaction updated = dialog.getTransaction();

        QString errorMessage;
        if (!AppContext::instance()
                 .transactionService()
                 .updateTransaction(updated, &errorMessage)) {
            QMessageBox::warning(this, "Lỗi", errorMessage);
        }
        // Bảng tự cập nhật qua signal transactionUpdated
    }
}

void TransactionPage::deleteTransactionById(int id)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Xác nhận",
        "Bạn có chắc chắn muốn xóa giao dịch này?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QString errorMessage;
        if (!AppContext::instance()
                 .transactionService()
                 .removeTransaction(id, &errorMessage)) {
            QMessageBox::warning(this, "Lỗi", errorMessage);
        }
        // Bảng tự cập nhật qua signal transactionRemoved
    }
}
