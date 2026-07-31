#include "CategoryPage.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
CategoryPage::CategoryPage(QWidget *parent)
    : QWidget(parent)
{
    // Tạo layout chính
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Tiêu đề
    QLabel *title = new QLabel("Category Management");

    // Nút thêm danh mục
    QPushButton *btnAddCategory =
        new QPushButton("Add Category");

    // Danh sách các danh mục
    QListWidget *categoryList = new QListWidget();

    // Một vài dữ liệu mẫu
    categoryList->addItem("Food");
    categoryList->addItem("Transportation");
    categoryList->addItem("Shopping");
    categoryList->addItem("Entertainment");
    categoryList->addItem("Education");

    // Thêm các widget vào layout
    layout->addWidget(title);
    layout->addWidget(btnAddCategory);
    layout->addWidget(categoryList);

    setLayout(layout);
}
