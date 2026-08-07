#ifndef BUDGETPAGE_H
#define BUDGETPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

class BudgetPage : public QWidget
{
    Q_OBJECT

public:
    explicit BudgetPage(QWidget *parent = nullptr);

public slots:
    void loadBudgets();
    void onFilterChanged();

private slots:
    void onAddBudget();

private:
    void editBudgetById(int id);
    void deleteBudgetById(int id);

    QVBoxLayout *budgetListLayout;
    class QComboBox *monthFilterCb;
    class QComboBox *sortCb;
    QPushButton *btnAdd;
    class QLabel *titleLabel;
    class QLabel *subtitleLabel;

    QWidget *budgetListWidget;
};

#endif // BUDGETPAGE_H
