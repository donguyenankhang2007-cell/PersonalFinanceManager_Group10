#include "TransactionDialog.h"
#include "../../app/AppContext.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>

TransactionDialog::TransactionDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Add Transaction");
    setMinimumWidth(400);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    amountSpin = new QDoubleSpinBox();
    amountSpin->setRange(1, 999999999);
    amountSpin->setDecimals(0);
    amountSpin->setSuffix(" VND");
    amountSpin->setValue(0);

    accountCombo = new QComboBox();
    categoryCombo = new QComboBox();

    dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("dd/MM/yyyy");

    noteEdit = new QLineEdit();
    noteEdit->setPlaceholderText("Enter note (optional)");

    typeCombo = new QComboBox();
    typeCombo->addItems({"expense", "income"});

    formLayout->addRow("Amount:", amountSpin);
    formLayout->addRow("Account:", accountCombo);
    formLayout->addRow("Category:", categoryCombo);
    formLayout->addRow("Date:", dateEdit);
    formLayout->addRow("Type:", typeCombo);
    formLayout->addRow("Note:", noteEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    setLayout(layout);

    loadAccounts();
    loadCategories();
}

void TransactionDialog::loadAccounts()
{
    accountCombo->clear();
    QVector<Account> accounts =
        AppContext::instance().accountRepository().getAllAccounts();

    for (const Account &acc : accounts) {
        accountCombo->addItem(acc.getName(), acc.getId());
    }
}

void TransactionDialog::loadCategories()
{
    categoryCombo->clear();
    QVector<Category> categories =
        AppContext::instance().categoryRepository().getAllCategories();

    for (const Category &cat : categories) {
        QString display = QString("%1 (%2)")
                              .arg(cat.getName())
                              .arg(cat.typeToString());
        categoryCombo->addItem(display, cat.getId());
    }
}

Transaction TransactionDialog::getTransaction() const
{
    int accountId = accountCombo->currentData().toInt();
    int categoryId = categoryCombo->currentData().toInt();

    return Transaction(
        0,
        accountId,
        categoryId,
        amountSpin->value(),
        dateEdit->date(),
        noteEdit->text(),
        typeCombo->currentText());
}
