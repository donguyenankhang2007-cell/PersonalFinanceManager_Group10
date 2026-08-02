#include "AccountDialog.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>

AccountDialog::AccountDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Add Account");
    setMinimumWidth(380);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("e.g. Cash, Bank, Momo...");

    balanceSpin = new QDoubleSpinBox();
    balanceSpin->setRange(0, 999999999);
    balanceSpin->setDecimals(0);
    balanceSpin->setSuffix(" VND");
    balanceSpin->setValue(0);

    descriptionEdit = new QLineEdit();
    descriptionEdit->setPlaceholderText("Description (optional)");

    formLayout->addRow("Account Name:", nameEdit);
    formLayout->addRow("Initial Balance:", balanceSpin);
    formLayout->addRow("Description:", descriptionEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    setLayout(layout);
}

Account AccountDialog::getAccount() const
{
    return Account(0, nameEdit->text(),
                   balanceSpin->value(),
                   descriptionEdit->text());
}
