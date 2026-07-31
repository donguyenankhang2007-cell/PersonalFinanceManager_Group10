// ============================================
// File: src/ui/pages/CategoryPage.cpp
// Nguoi viet: Viet Tuong (GUI Developer)
// Mo ta: Trang quan ly danh muc — CRUD day du
//        Ket noi CategoryRepository
// ============================================
#include "CategoryPage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>
#include <QLabel>

#include "core/repositories/CategoryRepository.h"
#include "dialogs/CategoryDialog.h"

CategoryPage::CategoryPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

// ==================== SETUP GIAO DIEN ====================
void CategoryPage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(15);

    // Tieu de
    QLabel *title = new QLabel("Quan ly Danh muc");
    title->setStyleSheet("color: #1A237E; font-size: 22px; font-weight: bold;");
    QLabel *subtitle = new QLabel("Quan ly cac danh muc thu chi cua ban");
    subtitle->setStyleSheet("color: #757575; font-size: 13px;");
    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    // === CAC NUT HANH DONG ===
    QHBoxLayout *actionLayout = new QHBoxLayout();

    m_btnAdd = new QPushButton("+ Them danh muc");
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

    // === BANG DANH MUC ===
    m_table = new QTableWidget();
    m_table->setColumnCount(5);
    m_table->setHorizontalHeaderLabels(
        {"ID", "Ten danh muc", "Loai", "Mau", "Icon"});
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->verticalHeader()->setVisible(false);
    m_table->setAlternatingRowColors(true);
    m_table->setStyleSheet("alternate-background-color: #F8F9FA;");
    m_table->setColumnHidden(0, true);
    mainLayout->addWidget(m_table, 1);

    // Ket noi signal/slot
    connect(m_btnAdd,    &QPushButton::clicked, this, &CategoryPage::onAddClicked);
    connect(m_btnEdit,   &QPushButton::clicked, this, &CategoryPage::onEditClicked);
    connect(m_btnDelete, &QPushButton::clicked, this, &CategoryPage::onDeleteClicked);
}

// ==================== TAI DU LIEU ====================
void CategoryPage::loadData()
{
    CategoryRepository catRepo;
    QVector<Category> categories = catRepo.getAllCategories();

    m_table->setRowCount(categories.size());

    for (int i = 0; i < categories.size(); ++i) {
        const Category &cat = categories[i];

        // Cot ID (an)
        m_table->setItem(i, 0,
            new QTableWidgetItem(QString::number(cat.getId())));

        // Cot Ten
        m_table->setItem(i, 1,
            new QTableWidgetItem(cat.getName()));

        // Cot Loai (mau xanh/do)
        QString typeText = (cat.getType() == CategoryType::Income)
            ? "Thu nhap" : "Chi tieu";
        QTableWidgetItem *typeItem = new QTableWidgetItem(typeText);
        typeItem->setForeground(
            cat.getType() == CategoryType::Income
                ? QColor("#2E7D32") : QColor("#C62828"));
        m_table->setItem(i, 2, typeItem);

        // Cot Mau
        QTableWidgetItem *colorItem = new QTableWidgetItem(cat.getColor());
        if (!cat.getColor().isEmpty()) {
            colorItem->setBackground(QColor(cat.getColor()));
        }
        m_table->setItem(i, 3, colorItem);

        // Cot Icon
        m_table->setItem(i, 4,
            new QTableWidgetItem(cat.getIcon()));
    }
}

// ==================== THEM DANH MUC ====================
void CategoryPage::onAddClicked()
{
    CategoryDialog dialog(this);
    dialog.setWindowTitle("Them danh muc moi");

    if (dialog.exec() == QDialog::Accepted) {
        Category newCat = dialog.getCategory();
        CategoryRepository repo;
        repo.addCategory(newCat);
        loadData();
    }
}

// ==================== SUA DANH MUC ====================
void CategoryPage::onEditClicked()
{
    int currentRow = m_table->currentRow();
    if (currentRow < 0) {
        QMessageBox::information(this, "Thong bao",
            "Vui long chon mot danh muc trong bang de sua.");
        return;
    }

    int catId = m_table->item(currentRow, 0)->text().toInt();
    CategoryRepository repo;
    Category cat = repo.getCategoryById(catId);

    CategoryDialog dialog(cat, this);
    dialog.setWindowTitle("Sua danh muc");

    if (dialog.exec() == QDialog::Accepted) {
        Category updatedCat = dialog.getCategory();
        repo.updateCategory(updatedCat);
        loadData();
    }
}

// ==================== XOA DANH MUC ====================
void CategoryPage::onDeleteClicked()
{
    int currentRow = m_table->currentRow();
    if (currentRow < 0) {
        QMessageBox::information(this, "Thong bao",
            "Vui long chon mot danh muc trong bang de xoa.");
        return;
    }

    int catId = m_table->item(currentRow, 0)->text().toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Xac nhan xoa",
        "Ban co chac chan muon xoa danh muc nay?\n"
        "Cac giao dich thuoc danh muc nay co the bi anh huong.",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        CategoryRepository repo;
        repo.deleteCategory(catId);
        loadData();
    }
}
