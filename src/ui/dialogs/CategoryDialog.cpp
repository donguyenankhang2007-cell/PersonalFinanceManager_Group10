#include "CategoryDialog.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QDialogButtonBox>

CategoryDialog::CategoryDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Add Category");
    setMinimumWidth(350);

    QVBoxLayout *layout = new QVBoxLayout(this);

    QFormLayout *formLayout = new QFormLayout();

    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Enter category name");

    typeCombo = new QComboBox();
    typeCombo->addItems({"Expense", "Income"});

    colorEdit = new QLineEdit();
    colorEdit->setPlaceholderText("#ff5733");

    iconEdit = new QLineEdit();
    iconEdit->setPlaceholderText("icon name (optional)");

    formLayout->addRow("Name:", nameEdit);
    formLayout->addRow("Type:", typeCombo);
    formLayout->addRow("Color:", colorEdit);
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
    CategoryType type = (typeCombo->currentText() == "Income")
                            ? CategoryType::Income
                            : CategoryType::Expense;

    return Category(0, nameEdit->text(), type,
                    colorEdit->text(), iconEdit->text());
}
