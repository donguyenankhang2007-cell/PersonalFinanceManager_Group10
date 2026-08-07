#ifndef BUDGETDIALOG_H
#define BUDGETDIALOG_H

#include <QDialog>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QSpinBox>

#include "../../core/models/Budget.h"

class BudgetDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor cho THEM MOI
    explicit BudgetDialog(QWidget *parent = nullptr);

    // Constructor cho SUA (pre-fill du lieu)
    BudgetDialog(const Budget& budget, QWidget *parent = nullptr);

    Budget getBudget() const;

private:
    int m_id;
    QComboBox *categoryCombo;
    QDoubleSpinBox *amountSpin;
    QSpinBox *monthSpin;
    QSpinBox *yearSpin;
};

#endif // BUDGETDIALOG_H
