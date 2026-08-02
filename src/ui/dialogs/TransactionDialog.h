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

    // Constructor cho SUA (pre-fill du lieu)
    TransactionDialog(const Transaction& transaction, QWidget *parent = nullptr);

    Transaction getTransaction() const;

private:
    int m_id;
    QDoubleSpinBox *amountSpin;
    QComboBox *accountCombo;
    QComboBox *categoryCombo;
    QDateEdit *dateEdit;
    QLineEdit *noteEdit;
    QComboBox *typeCombo;

    void loadAccounts(int selectedAccountId = -1);
    void loadCategories(int selectedCategoryId = -1);
};

#endif // TRANSACTIONDIALOG_H
