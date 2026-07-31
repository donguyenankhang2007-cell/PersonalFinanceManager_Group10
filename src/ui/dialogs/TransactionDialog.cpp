#include "TransactionDialog.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QPushButton>

TransactionDialog::TransactionDialog(QWidget *parent)
    : QDialog(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title = new QLabel("Add Transaction");

    QLineEdit *amount = new QLineEdit();
    amount->setPlaceholderText("Enter amount");

    QComboBox *category = new QComboBox();
    category->addItems({
        "Food",
        "Transportation",
        "Shopping",
        "Entertainment"
    });

    QDateEdit *date = new QDateEdit();
    date->setCalendarPopup(true);

    QComboBox *type = new QComboBox();
    type->addItems({
        "Income",
        "Expense"
    });

    QPushButton *saveButton =
        new QPushButton("Save");

    layout->addWidget(title);
    layout->addWidget(amount);
    layout->addWidget(category);
    layout->addWidget(date);
    layout->addWidget(type);
    layout->addWidget(saveButton);

    setLayout(layout);
}
