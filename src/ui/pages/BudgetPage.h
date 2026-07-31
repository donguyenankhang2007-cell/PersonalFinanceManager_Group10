#ifndef BUDGETPAGE_H
#define BUDGETPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QPushButton>

class BudgetPage : public QWidget
{
    Q_OBJECT

public:
    explicit BudgetPage(QWidget *parent = nullptr);

public slots:
    void loadBudgets();

private slots:
    void onAddBudget();
    void onDeleteBudget();

private:
    QVBoxLayout *budgetListLayout;
    QComboBox *categoryCombo;
    QDoubleSpinBox *amountSpin;
    QSpinBox *monthSpin;
    QSpinBox *yearSpin;
    QPushButton *btnAdd;

    QWidget *budgetListWidget;
};

#endif // BUDGETPAGE_H
