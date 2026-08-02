#include "BudgetDialog.h"
#include "../../app/AppContext.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QDate>

BudgetDialog::BudgetDialog(QWidget *parent)
    : QDialog(parent)
    , m_id(0)
{
    setWindowTitle("Thêm Ngân Sách");
    setMinimumWidth(380);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    categoryCombo = new QComboBox();

    amountSpin = new QDoubleSpinBox();
    amountSpin->setRange(1, 999999999);
    amountSpin->setDecimals(0);
    amountSpin->setSuffix(" VND");
    amountSpin->setValue(1000000);

    monthSpin = new QSpinBox();
    monthSpin->setRange(1, 12);
    monthSpin->setValue(QDate::currentDate().month());

    yearSpin = new QSpinBox();
    yearSpin->setRange(2020, 2030);
    yearSpin->setValue(QDate::currentDate().year());

    formLayout->addRow("Danh mục:", categoryCombo);
    formLayout->addRow("Giới hạn:", amountSpin);
    formLayout->addRow("Tháng:", monthSpin);
    formLayout->addRow("Năm:", yearSpin);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    setLayout(layout);

    // Load categories (chỉ expense categories để đặt ngân sách chi tiêu)
    QVector<Category> categories =
        AppContext::instance().categoryService().getAllCategories();
    for (const Category &cat : categories) {
        if (cat.getType() == CategoryType::Expense)
            categoryCombo->addItem(cat.getName(), cat.getId());
    }
}

BudgetDialog::BudgetDialog(const Budget& budget, QWidget *parent)
    : QDialog(parent)
    , m_id(budget.getId())
{
    setWindowTitle("Sửa Ngân Sách");
    setMinimumWidth(380);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    categoryCombo = new QComboBox();

    amountSpin = new QDoubleSpinBox();
    amountSpin->setRange(1, 999999999);
    amountSpin->setDecimals(0);
    amountSpin->setSuffix(" VND");
    amountSpin->setValue(budget.getAmount());

    monthSpin = new QSpinBox();
    monthSpin->setRange(1, 12);
    monthSpin->setValue(budget.getMonth());

    yearSpin = new QSpinBox();
    yearSpin->setRange(2020, 2030);
    yearSpin->setValue(budget.getYear());

    formLayout->addRow("Danh mục:", categoryCombo);
    formLayout->addRow("Giới hạn:", amountSpin);
    formLayout->addRow("Tháng:", monthSpin);
    formLayout->addRow("Năm:", yearSpin);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    setLayout(layout);

    QVector<Category> categories =
        AppContext::instance().categoryService().getAllCategories();
    int selectIndex = 0;
    for (int i = 0; i < categories.size(); ++i) {
        if (categories[i].getType() != CategoryType::Expense)
            continue;
        categoryCombo->addItem(categories[i].getName(), categories[i].getId());
        if (categories[i].getId() == budget.getCategoryId())
            selectIndex = categoryCombo->count() - 1;
    }
    if (categoryCombo->count() > 0)
        categoryCombo->setCurrentIndex(selectIndex);
}

Budget BudgetDialog::getBudget() const
{
    return Budget(m_id,
                  categoryCombo->currentData().toInt(),
                  amountSpin->value(),
                  monthSpin->value(),
                  yearSpin->value());
}
