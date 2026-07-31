#ifndef BUDGETPAGE_H
#define BUDGETPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QPushButton>

class BudgetPage : public QWidget
{
    Q_OBJECT

public:
    explicit BudgetPage(QWidget *parent = nullptr);

public slots:
    // Tai va hien thi du lieu ngan sach
    void loadData();

private slots:
    void onAddClicked();

private:
    void setupUI();
    QWidget* createBudgetCard(const QString &categoryName,
                               double budgetAmount, double spentAmount);

    QVBoxLayout *m_cardsLayout;
    QPushButton *m_btnAdd;
    QScrollArea *m_scrollArea;
};

#endif // BUDGETPAGE_H
