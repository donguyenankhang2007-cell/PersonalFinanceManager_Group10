#include "CategoryDialog.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>

CategoryDialog::CategoryDialog(QWidget *parent)
    : QDialog(parent)
    , m_id(0)
{
    setWindowTitle("Thêm Danh Mục");
    setMinimumWidth(350);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();

    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Nhập tên danh mục");

    typeCombo = new QComboBox();
    typeCombo->addItem("Chi Tiêu", "Expense");
    typeCombo->addItem("Thu Nhập", "Income");

    colorEdit = new QLineEdit();
    colorEdit->setPlaceholderText("#ff5733");

    iconEdit = new QLineEdit();
    iconEdit->setPlaceholderText("tên icon (tùy chọn)");

    formLayout->addRow("Tên:", nameEdit);
    formLayout->addRow("Loại:", typeCombo);
    formLayout->addRow("Màu sắc:", colorEdit);
    formLayout->addRow("Icon:", iconEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    setLayout(layout);
}

CategoryDialog::CategoryDialog(const Category& category, QWidget *parent)
    : QDialog(parent)
    , m_id(category.getId())
{
    setWindowTitle("Sửa Danh Mục");
    setMinimumWidth(350);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();

    nameEdit = new QLineEdit();
    nameEdit->setText(category.getName());

    typeCombo = new QComboBox();
    typeCombo->addItem("Chi Tiêu", "Expense");
    typeCombo->addItem("Thu Nhập", "Income");
    typeCombo->setCurrentIndex(category.getType() == CategoryType::Income ? 1 : 0);

    colorEdit = new QLineEdit();
    colorEdit->setText(category.getColor());

    iconEdit = new QLineEdit();
    iconEdit->setText(category.getIcon());

    formLayout->addRow("Tên:", nameEdit);
    formLayout->addRow("Loại:", typeCombo);
    formLayout->addRow("Màu sắc:", colorEdit);
    formLayout->addRow("Icon:", iconEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    setLayout(layout);
}

Category CategoryDialog::getCategory() const
{
    CategoryType type = (typeCombo->currentData().toString() == "Income")
                            ? CategoryType::Income
                            : CategoryType::Expense;

    return Category(m_id, nameEdit->text(), type,
                    colorEdit->text(), iconEdit->text());
}
