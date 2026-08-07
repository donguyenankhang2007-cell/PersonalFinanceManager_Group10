#ifndef RECURRINGTRANSACTIONDIALOG_H
#define RECURRINGTRANSACTIONDIALOG_H

#include <QDialog>
#include "../../core/models/RecurringTransaction.h"

class QLineEdit;
class QDoubleSpinBox;
class QComboBox;
class QDateEdit;
class QCheckBox;
class QLabel;

class RecurringTransactionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RecurringTransactionDialog(QWidget *parent = nullptr);
    RecurringTransactionDialog(const RecurringTransaction& recurring,
                               QWidget *parent = nullptr);

    RecurringTransaction getRecurringTransaction() const;

private:
    void updateTypeLabel();

    int m_id;
    QLineEdit *noteEdit;
    QDoubleSpinBox *amountSpin;
    QComboBox *categoryCombo;
    QComboBox *accountCombo;
    QComboBox *frequencyCombo;
    QDateEdit *nextDateEdit;
    QCheckBox *endCheck;
    QDateEdit *endDateEdit;
    QCheckBox *activeCheck;
    QLabel *typeLabel;
};

#endif // RECURRINGTRANSACTIONDIALOG_H
