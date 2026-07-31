#include "CategoryDialog.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

CategoryDialog::CategoryDialog(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title =
        new QLabel("Add Category");

    QLineEdit *categoryName =
        new QLineEdit();

    categoryName->setPlaceholderText(
        "Enter category name");

    QPushButton *saveButton =
        new QPushButton("Save");

    layout->addWidget(title);
    layout->addWidget(categoryName);
    layout->addWidget(saveButton);

    setLayout(layout);
}
