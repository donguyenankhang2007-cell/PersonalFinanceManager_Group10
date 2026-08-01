#ifndef TRANSACTIONDIALOG_H
#define TRANSACTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QDoubleSpinBox>

#include "../../core/models/Transaction.h"

class TransactionDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor cho THEM MOI
    explicit TransactionDialog(QWidget *parent = nullptr);

    Transaction getTransaction() const;

private:
    QDoubleSpinBox *amountSpin;
    QComboBox *accountCombo;
    QComboBox *categoryCombo;
    QDateEdit *dateEdit;
    QLineEdit *noteEdit;
    QComboBox *typeCombo;

    void loadAccounts();
    void loadCategories();
};

#endif // TRANSACTIONDIALOG_H
