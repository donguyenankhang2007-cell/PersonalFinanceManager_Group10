// ============================================
// File: src/ui/pages/RecurringPage.cpp
// Mo ta: Trang giao dich dinh ky — hien thi cac quy tac
//        tu dong sinh giao dich, nut tao ngay cac ky den han.
// ============================================
#include "RecurringPage.h"
#include "../dialogs/RecurringTransactionDialog.h"
#include "../RowActions.h"
#include "../theme/ThemeManager.h"
#include "../../app/AppContext.h"
#include "../../utils/MoneyUtils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QMap>

RecurringPage::RecurringPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(15);

    // Tiêu đề
    QHBoxLayout *header = new QHBoxLayout();
    QVBoxLayout *titleBox = new QVBoxLayout();
    titleBox->setSpacing(2);
    QLabel *title = new QLabel("Giao Dịch Định Kỳ");
    title->setObjectName("pageTitle");
    QLabel *subtitle = new QLabel("Tự động sinh giao dịch theo lịch");
    subtitle->setObjectName("pageSubtitle");
    titleBox->addWidget(title);
    titleBox->addWidget(subtitle);

    btnGenerate = new QPushButton("Sinh giao dịch đến hạn");
    btnGenerate->setObjectName("secondaryBtn");
    btnGenerate->setCursor(Qt::PointingHandCursor);

    btnAdd = new QPushButton("+ Thêm GD Định Kỳ");
    btnAdd->setObjectName("primaryBtn");
    btnAdd->setCursor(Qt::PointingHandCursor);

    header->addLayout(titleBox);
    header->addStretch();
    header->addWidget(btnGenerate);
    header->addWidget(btnAdd);
    layout->addLayout(header);

    // Bảng
    table = new QTableWidget();
    table->setColumnCount(9);
    table->setHorizontalHeaderLabels(
        {"ID", "Ghi Chú", "Danh Mục", "Tài Khoản", "Số Tiền (VND)",
         "Tần Suất", "Ngày Kế Tiếp", "Ngày Kết Thúc", "Hành Động"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    table->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Fixed);
    table->setColumnWidth(8, 92);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setVisible(false);

    layout->addWidget(table);

    setLayout(layout);

    // Kết nối
    connect(btnAdd, &QPushButton::clicked, this, &RecurringPage::onAddRecurring);
    connect(btnGenerate, &QPushButton::clicked, this, &RecurringPage::onGenerateNow);

    connect(table, &QTableWidget::cellDoubleClicked,
            this, [this](int row, int) {
                if (row >= 0) {
                    int id = table->item(row, 0)->text().toInt();
                    editRecurringById(id);
                }
            });

    // Observer: tự cập nhật khi service phát signal
    RecurringTransactionService &service =
        AppContext::instance().recurringTransactionService();
    connect(&service, &RecurringTransactionService::recurringAdded,
            this, &RecurringPage::loadRecurring);
    connect(&service, &RecurringTransactionService::recurringUpdated,
            this, &RecurringPage::loadRecurring);
    connect(&service, &RecurringTransactionService::recurringRemoved,
            this, &RecurringPage::loadRecurring);

    // Đổi mật độ bảng → áp chiều cao hàng
    connect(&ThemeManager::instance(), &ThemeManager::densityChanged,
            this, &RecurringPage::loadRecurring);

    loadRecurring();
}

void RecurringPage::loadRecurring()
{
    table->setRowCount(0);

    QVector<RecurringTransaction> items =
        AppContext::instance().recurringTransactionService().getAll();

    QVector<Category> categories =
        AppContext::instance().categoryService().getAllCategories();
    QMap<int, QString> catNames;
    for (const Category &c : categories)
        catNames[c.getId()] = c.getName();

    QVector<Account> accounts =
        AppContext::instance().accountService().getAllAccounts();
    QMap<int, QString> accNames;
    for (const Account &a : accounts)
        accNames[a.getId()] = a.getName();

    int rowHeight = ThemeManager::instance().tableRowHeight();

    for (const RecurringTransaction &r : items) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setRowHeight(row, rowHeight);

        table->setItem(row, 0,
                       new QTableWidgetItem(QString::number(row + 1)));
        table->setItem(row, 1, new QTableWidgetItem(r.getNote()));
        table->setItem(row, 2,
                       new QTableWidgetItem(catNames.value(r.getCategoryId(), "Không xác định")));
        table->setItem(row, 3,
                       new QTableWidgetItem(accNames.value(r.getAccountId(), "Không xác định")));

        QTableWidgetItem *amountItem = new QTableWidgetItem(
            MoneyUtils::formatVND(r.getAmount()));
        amountItem->setForeground(r.getType() == "income"
                                      ? QColor("#059669")
                                      : QColor("#E11D48"));
        table->setItem(row, 4, amountItem);

        table->setItem(row, 5, new QTableWidgetItem(r.frequencyDisplay()));
        table->setItem(row, 6,
                       new QTableWidgetItem(r.getNextDate().toString("dd/MM/yyyy")));
        table->setItem(row, 7,
                       new QTableWidgetItem(r.hasEndDate()
                                                ? r.getEndDate().toString("dd/MM/yyyy")
                                                : "—"));

        table->setCellWidget(row, 8,
            RowActions::create(r.getId(),
                [this](int id) { editRecurringById(id); },
                [this](int id) { deleteRecurringById(id); },
                this));
    }
}

void RecurringPage::onAddRecurring()
{
    RecurringTransactionDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        QString errorMessage;
        if (!AppContext::instance()
                 .recurringTransactionService()
                 .add(dialog.getRecurringTransaction(), &errorMessage)) {
            QMessageBox::warning(this, "Lỗi", errorMessage);
        }
    }
}

void RecurringPage::editRecurringById(int id)
{
    // Lấy bản ghi hiện tại từ repository (qua service lấy toàn bộ)
    QVector<RecurringTransaction> all =
        AppContext::instance().recurringTransactionService().getAll();
    RecurringTransaction current;
    bool found = false;
    for (const RecurringTransaction &r : all) {
        if (r.getId() == id) {
            current = r;
            found = true;
            break;
        }
    }
    if (!found) {
        QMessageBox::warning(this, "Lỗi", "Không tìm thấy giao dịch định kỳ!");
        return;
    }

    RecurringTransactionDialog dialog(current, this);

    if (dialog.exec() == QDialog::Accepted) {
        QString errorMessage;
        if (!AppContext::instance()
                 .recurringTransactionService()
                 .update(dialog.getRecurringTransaction(), &errorMessage)) {
            QMessageBox::warning(this, "Lỗi", errorMessage);
        }
    }
}

void RecurringPage::deleteRecurringById(int id)
{
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Xác nhận",
        "Bạn có chắc chắn muốn xóa giao dịch định kỳ này không?\n"
        "(Các giao dịch đã sinh ra trước đó vẫn được giữ lại.)",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        QString errorMessage;
        if (!AppContext::instance()
                 .recurringTransactionService()
                 .remove(id, &errorMessage)) {
            QMessageBox::warning(this, "Lỗi", errorMessage);
        }
    }
}

void RecurringPage::onGenerateNow()
{
    int count = AppContext::instance().recurringTransactionService()
                    .generateDueTransactions();

    if (count > 0) {
        QMessageBox::information(
            this, "Sinh giao dịch",
            QString("Đã sinh %1 giao dịch.").arg(count));
    } else {
        QMessageBox::information(
            this, "Sinh giao dịch",
            "Chưa có giao dịch định kỳ nào đến hạn.");
    }
    loadRecurring();
}
