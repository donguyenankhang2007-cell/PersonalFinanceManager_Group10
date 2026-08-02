// ============================================
// File: src/ui/pages/AccountPage.cpp
// Mo ta: Quan ly tai khoan. Moi hang co icon Edit/Delete
//        (RowActions) thay vi nut tren dau bang.
// ============================================
#include "AccountPage.h"
#include "../dialogs/AccountDialog.h"
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
#include <QGraphicsDropShadowEffect>

AccountPage::AccountPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(15);

    // Tiêu đề
    QHBoxLayout *header = new QHBoxLayout();
    QVBoxLayout *titleBox = new QVBoxLayout();
    titleBox->setSpacing(2);
    m_titleLabel = new QLabel("Tài Khoản");
    m_titleLabel->setObjectName("pageTitle");
    m_subtitleLabel = new QLabel("Quản lý tài khoản và số dư");
    m_subtitleLabel->setObjectName("pageSubtitle");
    titleBox->addWidget(m_titleLabel);
    titleBox->addWidget(m_subtitleLabel);

    m_btnAdd = new QPushButton("+ Thêm Tài Khoản");
    m_btnAdd->setObjectName("primaryBtn");
    m_btnAdd->setCursor(Qt::PointingHandCursor);
    header->addLayout(titleBox);
    header->addStretch();
    header->addWidget(m_btnAdd);
    layout->addLayout(header);

    // Bảng danh sách accounts
    m_table = new QTableWidget();
    m_table->setColumnCount(5);
    m_table->setHorizontalHeaderLabels(
        {"ID", "Tên", "Số Dư (VND)", "Mô Tả", "Hành Động"});
    m_table->horizontalHeader()->setSectionResizeMode(
        QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(
        4, QHeaderView::Fixed);
    m_table->setColumnWidth(4, 110);
    m_table->horizontalHeader()->setSectionResizeMode(
        0, QHeaderView::ResizeToContents);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
    m_table->verticalHeader()->setVisible(false);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 15));
    shadow->setOffset(0, 4);
    m_table->setGraphicsEffect(shadow);

    layout->addWidget(m_table);

    setLayout(layout);

    // Kết nối signals
    connect(m_btnAdd, &QPushButton::clicked, this, &AccountPage::onAddAccount);

    // Double-click hàng → sửa nhanh
    connect(m_table, &QTableWidget::cellDoubleClicked,
            this, [this](int row, int) {
                if (row >= 0) {
                    int id = m_table->item(row, 0)->text().toInt();
                    editAccountById(id);
                }
            });

    // Tự cập nhật số dư khi giao dịch thay đổi
    TransactionService &txService = AppContext::instance().transactionService();
    connect(&txService, &TransactionService::transactionAdded,
            this, &AccountPage::loadAccounts);
    connect(&txService, &TransactionService::transactionUpdated,
            this, &AccountPage::loadAccounts);
    connect(&txService, &TransactionService::transactionRemoved,
            this, &AccountPage::loadAccounts);

    // Đổi mật độ bảng → áp chiều cao hàng
    connect(&ThemeManager::instance(), &ThemeManager::densityChanged,
            this, &AccountPage::loadAccounts);

    loadAccounts();
}

void AccountPage::loadAccounts()
{
    m_table->setHorizontalHeaderLabels({
        "ID", "Tên", "Số Dư (VND)", "Mô Tả", "Hành Động"
    });

    m_table->setRowCount(0);

    QVector<Account> accounts =
        AppContext::instance().accountService().getAllAccounts();

    int rowHeight = ThemeManager::instance().tableRowHeight();

    for (const Account &acc : accounts) {
        int row = m_table->rowCount();
        m_table->insertRow(row);
        m_table->setRowHeight(row, rowHeight);

        m_table->setItem(row, 0,
                       new QTableWidgetItem(QString::number(acc.getId())));
        m_table->setItem(row, 1, new QTableWidgetItem(acc.getName()));
        m_table->setItem(row, 2,
                       new QTableWidgetItem(MoneyUtils::formatVND(acc.getBalance())));
        m_table->setItem(row, 3,
                       new QTableWidgetItem(acc.getDescription()));

        m_table->setCellWidget(row, 4,
            RowActions::create(acc.getId(),
                [this](int id) { editAccountById(id); },
                [this](int id) { deleteAccountById(id); },
                this));
    }
}

void AccountPage::onAddAccount()
{
    AccountDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        Account acc = dialog.getAccount();

        if (acc.getName().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Lỗi",
                                 "Tên tài khoản không được để trống!");
            return;
        }

        if (AppContext::instance().accountService().addAccount(acc)) {
            loadAccounts();
        } else {
            QMessageBox::warning(this, "Lỗi",
                                 "Thêm tài khoản thất bại!");
        }
    }
}

void AccountPage::editAccountById(int id)
{
    Account account =
        AppContext::instance().accountService().getAccountById(id);

    AccountDialog dialog(account, this);

    if (dialog.exec() == QDialog::Accepted) {
        Account updated = dialog.getAccount();

        if (updated.getName().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Lỗi",
                                 "Tên tài khoản không được để trống!");
            return;
        }

        if (AppContext::instance().accountService().updateAccount(updated)) {
            loadAccounts();
        } else {
            QMessageBox::warning(this, "Lỗi",
                                 "Cập nhật tài khoản thất bại!");
        }
    }
}

void AccountPage::deleteAccountById(int id)
{
    Account account =
        AppContext::instance().accountService().getAccountById(id);
    QString name = account.getName();

    // Bảo vệ dữ liệu: không cho xóa account đang có giao dịch
    int txCount = AppContext::instance()
                      .transactionService()
                      .countTransactionsForAccount(id);

    if (txCount > 0) {
        QMessageBox::warning(
            this, "Không thể xóa",
            QString("Tài khoản '%1' đang có %2 giao dịch liên quan.\n"
                    "Vui lòng xóa các giao dịch đó trước.")
                .arg(name)
                .arg(txCount));
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Xác nhận",
        QString("Bạn có chắc chắn muốn xóa tài khoản '%1' không?").arg(name),
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (AppContext::instance().accountService().removeAccount(id)) {
            loadAccounts();
        } else {
            QMessageBox::warning(this, "Lỗi",
                                 "Xóa tài khoản thất bại!");
        }
    }
}
