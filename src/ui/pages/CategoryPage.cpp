#include "CategoryPage.h"
#include "../dialogs/CategoryDialog.h"
#include "../../app/AppContext.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

CategoryPage::CategoryPage(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(15);

    // Tiêu đề
    QLabel *title = new QLabel("Category Management");
    title->setStyleSheet(
        "font-size: 22px;"
        "font-weight: bold;"
        "color: #1e272e;"
        "padding-bottom: 10px;");

    // Buttons
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnAdd = new QPushButton("+ Add Category");
    btnDelete = new QPushButton("Delete Selected");
    btnDelete->setStyleSheet(
        "background-color: #ff3f34; color: white;"
        "border: none; padding: 10px 20px;"
        "font-size: 13px; font-weight: bold; border-radius: 5px;");

    btnLayout->addWidget(btnAdd);
    btnLayout->addWidget(btnDelete);
    btnLayout->addStretch();

    // Danh sách
    categoryList = new QListWidget();

    layout->addWidget(title);
    layout->addLayout(btnLayout);
    layout->addWidget(categoryList);

    setLayout(layout);

    // Kết nối signals
    connect(btnAdd, &QPushButton::clicked,
            this, &CategoryPage::onAddCategory);
    connect(btnDelete, &QPushButton::clicked,
            this, &CategoryPage::onDeleteCategory);

    // Load dữ liệu từ DB
    loadCategories();
}

void CategoryPage::loadCategories()
{
    categoryList->clear();

    QVector<Category> categories =
        AppContext::instance().categoryRepository().getAllCategories();

    for (const Category &cat : categories) {
        QString display = QString("%1  (%2)")
                              .arg(cat.getName())
                              .arg(cat.typeToString());

        QListWidgetItem *item = new QListWidgetItem(display);
        item->setData(Qt::UserRole, cat.getId());
        categoryList->addItem(item);
    }
}

void CategoryPage::onAddCategory()
{
    CategoryDialog dialog(this);

    if (dialog.exec() == QDialog::Accepted) {
        Category cat = dialog.getCategory();

        if (cat.getName().trimmed().isEmpty()) {
            QMessageBox::warning(this, "Error",
                                 "Category name cannot be empty!");
            return;
        }

        if (AppContext::instance().categoryRepository().addCategory(cat)) {
            loadCategories();
        } else {
            QMessageBox::warning(this, "Error",
                                 "Failed to add category!");
        }
    }
}

void CategoryPage::onDeleteCategory()
{
    QListWidgetItem *current = categoryList->currentItem();

    if (!current) {
        QMessageBox::information(this, "Info",
                                 "Please select a category to delete.");
        return;
    }

    int id = current->data(Qt::UserRole).toInt();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this, "Confirm",
        "Are you sure you want to delete this category?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (AppContext::instance().categoryRepository().deleteCategory(id)) {
            loadCategories();
        } else {
            QMessageBox::warning(this, "Error",
                                 "Failed to delete category!");
        }
    }
}
