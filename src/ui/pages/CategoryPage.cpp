// ============================================
// File: src/ui/pages/CategoryPage.cpp
// Mo ta: Quan ly danh muc dang bang voi mau cham mau
//        va icon Edit/Delete tren moi hang.
// ============================================
#include "CategoryPage.h"
#include "../dialogs/CategoryDialog.h"
#include "../RowActions.h"
#include "../theme/ThemeManager.h"
#include "../../app/AppContext.h"
#include "../../utils/StyleUtils.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QHeaderView>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>

CategoryPage::CategoryPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(15);

    // Tiêu đề
    QHBoxLayout *header = new QHBoxLayout();
    QVBoxLayout *titleBox = new QVBoxLayout();
    titleBox->setSpacing(2);
    titleLabel = new QLabel("Danh Mục");
    titleLabel->setObjectName("pageTitle");
    subtitleLabel = new QLabel("Quản lý danh mục thu chi");
    subtitleLabel->setObjectName("pageSubtitle");
    titleBox->addWidget(titleLabel);
    titleBox->addWidget(subtitleLabel);

    btnAdd = new QPushButton("+ Thêm Danh Mục");
    btnAdd->setObjectName("primaryBtn");
    btnAdd->setCursor(Qt::PointingHandCursor);
    header->addLayout(titleBox);
    header->addStretch();
    header->addWidget(btnAdd);
    layout->addLayout(header);

    // Bảng danh sách categories
    table = new QTableWidget();
    table->setColumnCount(3);
    table->setHorizontalHeaderLabels({"Tên Danh Mục", "Loại", "Hành Động"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    table->setColumnWidth(2, 110);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setSelectionMode(QAbstractItemView::SingleSelection);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setAlternatingRowColors(true);
    table->verticalHeader()->setVisible(false);

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(0, 0, 0, 15));
    shadow->setOffset(0, 4);
    table->setGraphicsEffect(shadow);

    layout->addWidget(table);

    setLayout(layout);

    // Kết nối signals
    connect(btnAdd, &QPushButton::clicked, this, &CategoryPage::onAddCategory);

    // Double-click hàng → sửa nhanh
    connect(table, &QTableWidget::cellDoubleClicked,
            this, [this](int row, int) {
                if (row >= 0) {
                    QTableWidgetItem *nameItem = table->item(row, 0);
                    if (nameItem)
                        editCategoryById(nameItem->data(Qt::UserRole).toInt());
                }
            });

    // Đổi mật độ bảng → áp chiều cao hàng
    connect(&ThemeManager::instance(), &ThemeManager::densityChanged,
            this, &CategoryPage::loadCategories);

    loadCategories();
}

void CategoryPage::loadCategories()
{
    table->setHorizontalHeaderLabels({
        "Tên Danh Mục", "Loại", "Hành Động"
    });

    table->setRowCount(0);

    QVector<Category> categories =
        AppContext::instance().categoryService().getAllCategories();

    int rowHeight = ThemeManager::instance().tableRowHeight();

    for (const Category &cat : categories) {
        int row = table->rowCount();
        table->insertRow(row);
        table->setRowHeight(row, rowHeight);

        // Tên kèm chấm màu theo màu của category
        QTableWidgetItem *nameItem =
            new QTableWidgetItem(QString("   %1").arg(cat.getName()));
        nameItem->setData(Qt::UserRole, cat.getId());
        QColor dotColor = StyleUtils::getCategoryColor(cat.getId());
        if (dotColor.isValid())
            nameItem->setForeground(dotColor);
        table->setItem(row, 0, nameItem);

        // Loại: thu nhập (xanh) / chi tiêu (đỏ)
        bool isInc = (cat.getType() == CategoryType::Income);
        QTableWidgetItem *typeItem = new QTableWidgetItem(
            isInc ? "Thu Nhập" : "Chi Tiêu");
        typeItem->setForeground(isInc
                                    ? QColor("#059669")
                                    : QColor("#E11D48"));
        table->setItem(row, 1, typeItem);

        table->setCellWidget(row, 2,
            RowActions::create(cat.getId(),
                [this](int id) { editCategoryById(id); },
                [this](int id) { deleteCategoryById(id); },
                this));
    }
}

void CategoryPage::onAddCategory()
{
    CategoryDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        Category cat = dialog.getCategory();

        if (cat.getName().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Lỗi",
                                 "Tên danh mục không được để trống!");
            return;
        }

        if (AppContext::instance().categoryService().addCategory(cat)) {
            loadCategories();
        } else {
            QMessageBox::warning(this, "Lỗi",
                                 "Thêm danh mục thất bại!");
        }
    }
}

void CategoryPage::editCategoryById(int id)
{
    Category category =
        AppContext::instance().categoryService().getCategoryById(id);

    CategoryDialog dialog(category, this);

    if (dialog.exec() == QDialog::Accepted) {
        Category updated = dialog.getCategory();

        if (updated.getName().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Lỗi",
                                 "Tên danh mục không được để trống!");
            return;
        }

        if (AppContext::instance().categoryService().updateCategory(updated)) {
            loadCategories();
        } else {
            QMessageBox::warning(this, "Lỗi",
                                 "Cập nhật danh mục thất bại!");
        }
    }
}

void CategoryPage::deleteCategoryById(int id)
{
    // Bảo vệ dữ liệu: không cho xóa category đang được dùng bởi giao dịch
    int txCount = AppContext::instance()
                      .transactionService()
                      .countTransactionsForCategory(id);

    if (txCount > 0) {
        QMessageBox::warning(
            this, "Không thể xóa",
            QString("Danh mục này đang được dùng trong %1 giao dịch.\n"
                    "Vui lòng xóa các giao dịch đó trước.")
                .arg(txCount));
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Xác nhận",
        "Bạn có chắc chắn muốn xóa danh mục này không?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (AppContext::instance().categoryService().removeCategory(id)) {
            loadCategories();
        } else {
            QMessageBox::warning(this, "Lỗi",
                                 "Xóa danh mục thất bại!");
        }
    }
}
