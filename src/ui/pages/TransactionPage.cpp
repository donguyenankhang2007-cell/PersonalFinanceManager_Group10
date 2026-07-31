// ============================================
// File: src/ui/pages/TransactionPage.cpp
// Nguoi viet: Viet Tuong (GUI Developer)
// Mo ta: Trang quan ly giao dich — CRUD day du
//        Ket noi TransactionRepository, CategoryRepository
//        va TransactionService de validate
// ============================================
#include "TransactionPage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QLabel>
#include <QFont>
#include <QMap>

#include "core/repositories/TransactionRepository.h"
#include "core/repositories/CategoryRepository.h"
#include "core/services/TransactionService.h"
#include "utils/MoneyUtils.h"
#include "utils/DateUtils.h"
#include "dialogs/TransactionDialog.h"

TransactionPage::TransactionPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

// ==================== SETUP GIAO DIEN ====================
void TransactionPage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(15);

    // Tieu de
    QLabel *title = new QLabel("Quan ly Giao dich");
    title->setStyleSheet("color: #1A237E; font-size: 22px; font-weight: bold;");
    QLabel *subtitle = new QLabel("Them, sua, xoa cac giao dich thu chi cua ban");
    subtitle->setStyleSheet("color: #757575; font-size: 13px;");
    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    // === CAC NUT HANH DONG ===
    QHBoxLayout *actionLayout = new QHBoxLayout();

    m_btnAdd = new QPushButton("+ Them giao dich");
    m_btnAdd->setObjectName("primaryBtn");
    m_btnAdd->setCursor(Qt::PointingHandCursor);

    m_btnEdit = new QPushButton("Sua");
    m_btnEdit->setObjectName("editBtn");
    m_btnEdit->setCursor(Qt::PointingHandCursor);

    m_btnDelete = new QPushButton("Xoa");
    m_btnDelete->setObjectName("deleteBtn");
    m_btnDelete->setCursor(Qt::PointingHandCursor);

    actionLayout->addWidget(m_btnAdd);
    actionLayout->addWidget(m_btnEdit);
    actionLayout->addWidget(m_btnDelete);
    actionLayout->addStretch();
    mainLayout->addLayout(actionLayout);

    // === BANG GIAO DICH ===
    m_table = new QTableWidget();
    m_table->setColumnCount(6);
    m_table->setHorizontalHeaderLabels(
        {"ID", "Ngay", "Loai", "Danh muc", "Ghi chu", "So tien"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->verticalHeader()->setVisible(false);
    m_table->setAlternatingRowColors(true);
    m_table->setStyleSheet("alternate-background-color: #F8F9FA;");
    // An cot ID (dung noi bo de lay ID khi edit/delete)
    m_table->setColumnHidden(0, true);
    mainLayout->addWidget(m_table, 1);

    // Ket noi signal/slot
    connect(m_btnAdd,    &QPushButton::clicked, this, &TransactionPage::onAddClicked);
    connect(m_btnEdit,   &QPushButton::clicked, this, &TransactionPage::onEditClicked);
    connect(m_btnDelete, &QPushButton::clicked, this, &TransactionPage::onDeleteClicked);
}

// ==================== TAI DU LIEU ====================
void TransactionPage::loadData()
{
    TransactionRepository transRepo;
    CategoryRepository catRepo;

    QVector<Transaction> transactions = transRepo.getAllTransactions();
    QVector<Category> categories = catRepo.getAllCategories();

    // Tao bang tra cuu ten danh muc
    QMap<int, QString> categoryNames;
    for (const Category &cat : categories) {
        categoryNames[cat.getId()] = cat.getName();
    }

    m_table->setRowCount(transactions.size());

    for (int i = 0; i < transactions.size(); ++i) {
        const Transaction &t = transactions[i];

        // Cot ID (an)
        m_table->setItem(i, 0,
            new QTableWidgetItem(QString::number(t.getId())));

        // Cot Ngay
        m_table->setItem(i, 1,
            new QTableWidgetItem(DateUtils::formatDisplay(t.getDate())));

        // Cot Loai
        QString typeText = (t.getType() == "income") ? "Thu nhap" : "Chi tieu";
        QTableWidgetItem *typeItem = new QTableWidgetItem(typeText);
        typeItem->setForeground(
            t.getType() == "income" ? QColor("#2E7D32") : QColor("#C62828"));
        QFont boldFont;
        boldFont.setBold(true);
        typeItem->setFont(boldFont);
        m_table->setItem(i, 2, typeItem);

        // Cot Danh muc
        QString catName = categoryNames.value(t.getCategoryId(), "Khong ro");
        m_table->setItem(i, 3, new QTableWidgetItem(catName));

        // Cot Ghi chu
        m_table->setItem(i, 4, new QTableWidgetItem(t.getNote()));

        // Cot So tien
        QTableWidgetItem *amountItem = new QTableWidgetItem(
            MoneyUtils::formatVND(t.getAmount()));
        amountItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        amountItem->setForeground(
            t.getType() == "income" ? QColor("#2E7D32") : QColor("#C62828"));
        m_table->setItem(i, 5, amountItem);
    }
}

// ==================== THEM GIAO DICH ====================
void TransactionPage::onAddClicked()
{
    TransactionDialog dialog(this);
    dialog.setWindowTitle("Them giao dich moi");

    if (dialog.exec() == QDialog::Accepted) {
        Transaction newTx = dialog.getTransaction();

        // Validate truoc khi luu (su dung TransactionService)
        TransactionService service;
        QString errorMsg;
        if (!service.validate(newTx, errorMsg)) {
            QMessageBox::warning(this, "Loi xac thuc", errorMsg);
            return;
        }

        // Luu vao database
        TransactionRepository repo;
        repo.addTransaction(newTx);

        // Lam moi bang
        loadData();
    }
}

// ==================== SUA GIAO DICH ====================
void TransactionPage::onEditClicked()
{
    int currentRow = m_table->currentRow();
    if (currentRow < 0) {
        QMessageBox::information(this, "Thong bao",
            "Vui long chon mot giao dich trong bang de sua.");
        return;
    }

    // Lay ID tu cot an
    int txId = m_table->item(currentRow, 0)->text().toInt();

    TransactionRepository repo;
    Transaction tx = repo.getTransactionById(txId);

    // Mo dialog voi du lieu hien tai
    TransactionDialog dialog(tx, this);
    dialog.setWindowTitle("Sua giao dich");

    if (dialog.exec() == QDialog::Accepted) {
        Transaction updatedTx = dialog.getTransaction();

        TransactionService service;
        QString errorMsg;
        if (!service.validate(updatedTx, errorMsg)) {
            QMessageBox::warning(this, "Loi xac thuc", errorMsg);
            return;
        }

        repo.updateTransaction(updatedTx);
        loadData();
    }
}

// ==================== XOA GIAO DICH ====================
void TransactionPage::onDeleteClicked()
{
    int currentRow = m_table->currentRow();
    if (currentRow < 0) {
        QMessageBox::information(this, "Thong bao",
            "Vui long chon mot giao dich trong bang de xoa.");
        return;
    }

    int txId = m_table->item(currentRow, 0)->text().toInt();

    // Hoi xac nhan truoc khi xoa
    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Xac nhan xoa",
        "Ban co chac chan muon xoa giao dich nay?\nHanh dong nay khong the hoan tac.",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        TransactionRepository repo;
        repo.deleteTransaction(txId);
        loadData();
    }
}
