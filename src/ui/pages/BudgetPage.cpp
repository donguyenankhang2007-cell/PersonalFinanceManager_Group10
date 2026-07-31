#include "BudgetPage.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QProgressBar>

BudgetPage::BudgetPage(QWidget *parent)
    : QWidget(parent)
{
    // Layout chính
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Tiêu đề
    QLabel *title = new QLabel("Monthly Budget");

    // Budget cho Food
    QLabel *foodLabel = new QLabel("Food");
    QProgressBar *foodBudget = new QProgressBar();
    foodBudget->setRange(0, 100);
    foodBudget->setValue(70);

    // Budget cho Transportation
    QLabel *transportLabel = new QLabel("Transportation");
    QProgressBar *transportBudget = new QProgressBar();
    transportBudget->setRange(0, 100);
    transportBudget->setValue(50);

    // Budget cho Shopping
    QLabel *shoppingLabel = new QLabel("Shopping");
    QProgressBar *shoppingBudget = new QProgressBar();
    shoppingBudget->setRange(0, 100);
    shoppingBudget->setValue(30);

    // Thêm vào layout
    layout->addWidget(title);

    layout->addWidget(foodLabel);
    layout->addWidget(foodBudget);

    layout->addWidget(transportLabel);
    layout->addWidget(transportBudget);

    layout->addWidget(shoppingLabel);
    layout->addWidget(shoppingBudget);

    setLayout(layout);
}