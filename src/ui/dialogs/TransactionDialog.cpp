#include "TransactionDialog.h"
#include "../../app/AppContext.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLabel>

TransactionDialog::TransactionDialog(QWidget *parent)
    : QDialog(parent)
    , m_id(0)
{
    setWindowTitle("Thêm Giao Dịch");
    setMinimumWidth(400);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    amountSpin = new QDoubleSpinBox();
    amountSpin->setRange(1, 999999999);
    amountSpin->setDecimals(0);
    amountSpin->setSuffix(" VND");
    amountSpin->setValue(0);

    accountCombo = new QComboBox();
    categoryCombo = new QComboBox();

    dateEdit = new QDateEdit(QDate::currentDate());
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("dd/MM/yyyy");

    noteEdit = new QLineEdit();
    noteEdit->setPlaceholderText("Nhập ghi chú (tùy chọn)");

    typeCombo = new QComboBox();
    typeCombo->addItem("Chi tiêu", "expense");
    typeCombo->addItem("Thu nhập", "income");

    formLayout->addRow("Số tiền:", amountSpin);
    formLayout->addRow("Tài khoản:", accountCombo);
    formLayout->addRow("Danh mục:", categoryCombo);
    formLayout->addRow("Ngày:", dateEdit);
    formLayout->addRow("Loại:", typeCombo);
    formLayout->addRow("Ghi chú:", noteEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    setLayout(layout);

    loadAccounts();
    loadCategories();
}

TransactionDialog::TransactionDialog(const Transaction& transaction, QWidget *parent)
    : QDialog(parent)
    , m_id(transaction.getId())
{
    setWindowTitle("Sửa Giao Dịch");
    setMinimumWidth(400);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    amountSpin = new QDoubleSpinBox();
    amountSpin->setRange(1, 999999999);
    amountSpin->setDecimals(0);
    amountSpin->setSuffix(" VND");
    amountSpin->setValue(transaction.getAmount());

    accountCombo = new QComboBox();
    categoryCombo = new QComboBox();

    dateEdit = new QDateEdit(transaction.getDate());
    dateEdit->setCalendarPopup(true);
    dateEdit->setDisplayFormat("dd/MM/yyyy");

    noteEdit = new QLineEdit();
    noteEdit->setText(transaction.getNote());

    typeCombo = new QComboBox();
    typeCombo->addItem("Chi tiêu", "expense");
    typeCombo->addItem("Thu nhập", "income");
    typeCombo->setCurrentIndex(transaction.isIncome() ? 1 : 0);

    formLayout->addRow("Số tiền:", amountSpin);
    formLayout->addRow("Tài khoản:", accountCombo);
    formLayout->addRow("Danh mục:", categoryCombo);
    formLayout->addRow("Ngày:", dateEdit);
    formLayout->addRow("Loại:", typeCombo);
    formLayout->addRow("Ghi chú:", noteEdit);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);

    setLayout(layout);

    loadAccounts(transaction.getAccountId());
    loadCategories(transaction.getCategoryId());
}

void TransactionDialog::loadAccounts(int selectedAccountId)
{
    accountCombo->clear();
    QVector<Account> accounts =
        AppContext::instance().accountService().getAllAccounts();

    int selectIndex = 0;
    for (int i = 0; i < accounts.size(); ++i) {
        accountCombo->addItem(accounts[i].getName(), accounts[i].getId());
        if (accounts[i].getId() == selectedAccountId)
            selectIndex = i;
    }
    if (accounts.size() > 0)
        accountCombo->setCurrentIndex(selectIndex);
}

void TransactionDialog::loadCategories(int selectedCategoryId)
{
    categoryCombo->clear();
    QVector<Category> categories =
        AppContext::instance().categoryService().getAllCategories();

    int selectIndex = 0;
    for (int i = 0; i < categories.size(); ++i) {
        QString display = QString("%1 (%2)")
                              .arg(categories[i].getName())
                              .arg(categories[i].typeToString());
        categoryCombo->addItem(display, categories[i].getId());
        if (categories[i].getId() == selectedCategoryId)
            selectIndex = i;
    }
    if (categories.size() > 0)
        categoryCombo->setCurrentIndex(selectIndex);
}

Transaction TransactionDialog::getTransaction() const
{
    int accountId = accountCombo->currentData().toInt();
    int categoryId = categoryCombo->currentData().toInt();

    return Transaction(
        m_id,
        accountId,
        categoryId,
        amountSpin->value(),
        dateEdit->date(),
        noteEdit->text(),
        typeCombo->currentData().toString());
}
