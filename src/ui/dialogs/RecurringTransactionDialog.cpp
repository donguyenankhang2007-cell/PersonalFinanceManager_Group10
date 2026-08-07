#include "RecurringTransactionDialog.h"
#include "../../app/AppContext.h"

#include <QVBoxLayout>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QDateEdit>
#include <QCheckBox>
#include <QLabel>
#include <QDate>

RecurringTransactionDialog::RecurringTransactionDialog(QWidget *parent)
    : QDialog(parent)
    , m_id(0)
{
    setWindowTitle("Thêm Giao Dịch Định Kỳ");
    setMinimumWidth(420);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    noteEdit = new QLineEdit();
    noteEdit->setPlaceholderText("VD: Tiền thuê nhà, Netflix, Lương...");

    amountSpin = new QDoubleSpinBox();
    amountSpin->setRange(1, 999999999);
    amountSpin->setDecimals(0);
    amountSpin->setSuffix(" VND");
    amountSpin->setValue(100000);

    categoryCombo = new QComboBox();
    accountCombo = new QComboBox();
    frequencyCombo = new QComboBox();
    frequencyCombo->addItems({"Hàng tháng", "Hàng tuần", "Hàng ngày", "Hàng năm"});

    nextDateEdit = new QDateEdit();
    nextDateEdit->setCalendarPopup(true);
    nextDateEdit->setDisplayFormat("dd/MM/yyyy");
    nextDateEdit->setDate(QDate::currentDate());

    endCheck = new QCheckBox("Không có ngày kết thúc");
    endCheck->setChecked(true);
    endCheck->setCursor(Qt::PointingHandCursor);

    endDateEdit = new QDateEdit();
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("dd/MM/yyyy");
    endDateEdit->setDate(QDate::currentDate().addYears(1));
    endDateEdit->setEnabled(false);

    activeCheck = new QCheckBox("Hoạt động (tự động sinh giao dịch)");
    activeCheck->setChecked(true);
    activeCheck->setCursor(Qt::PointingHandCursor);

    typeLabel = new QLabel();
    typeLabel->setObjectName("cardTitle");

    QHBoxLayout *endRow = new QHBoxLayout();
    endRow->addWidget(endCheck);
    endRow->addWidget(endDateEdit);

    formLayout->addRow("Ghi chú:", noteEdit);
    formLayout->addRow("Số tiền:", amountSpin);
    formLayout->addRow("Danh mục:", categoryCombo);
    formLayout->addRow("", typeLabel);
    formLayout->addRow("Tài khoản:", accountCombo);
    formLayout->addRow("Tần suất:", frequencyCombo);
    formLayout->addRow("Ngày kế tiếp:", nextDateEdit);
    formLayout->addRow("Kết thúc:", endRow);
    formLayout->addRow("", activeCheck);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);
    setLayout(layout);

    connect(endCheck, &QCheckBox::toggled, endDateEdit, &QWidget::setDisabled);
    connect(categoryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int) { updateTypeLabel(); });

    // Load categories (tất cả loại; type lấy theo category)
    QVector<Category> categories =
        AppContext::instance().categoryService().getAllCategories();
    for (const Category &cat : categories)
        categoryCombo->addItem(cat.getName(), cat.getId());

    // Tải danh sách tài khoản
    QVector<Account> accounts =
        AppContext::instance().accountService().getAllAccounts();
    for (const Account &acc : accounts)
        accountCombo->addItem(acc.getName(), acc.getId());

    updateTypeLabel();
}

RecurringTransactionDialog::RecurringTransactionDialog(
    const RecurringTransaction& recurring, QWidget *parent)
    : QDialog(parent)
    , m_id(recurring.getId())
{
    setWindowTitle("Sửa Giao Dịch Định Kỳ");
    setMinimumWidth(420);

    QVBoxLayout *layout = new QVBoxLayout(this);
    QFormLayout *formLayout = new QFormLayout();

    noteEdit = new QLineEdit(recurring.getNote());

    amountSpin = new QDoubleSpinBox();
    amountSpin->setRange(1, 999999999);
    amountSpin->setDecimals(0);
    amountSpin->setSuffix(" VND");
    amountSpin->setValue(recurring.getAmount());

    categoryCombo = new QComboBox();
    accountCombo = new QComboBox();
    frequencyCombo = new QComboBox();
    frequencyCombo->addItems({"Hàng tháng", "Hàng tuần", "Hàng ngày", "Hàng năm"});

    nextDateEdit = new QDateEdit();
    nextDateEdit->setCalendarPopup(true);
    nextDateEdit->setDisplayFormat("dd/MM/yyyy");
    nextDateEdit->setDate(recurring.getNextDate());

    endCheck = new QCheckBox("Không có ngày kết thúc");
    endCheck->setChecked(!recurring.hasEndDate());
    endCheck->setCursor(Qt::PointingHandCursor);

    endDateEdit = new QDateEdit();
    endDateEdit->setCalendarPopup(true);
    endDateEdit->setDisplayFormat("dd/MM/yyyy");
    endDateEdit->setDate(recurring.hasEndDate()
                             ? recurring.getEndDate()
                             : QDate::currentDate().addYears(1));
    endDateEdit->setEnabled(recurring.hasEndDate());

    activeCheck = new QCheckBox("Hoạt động (tự động sinh giao dịch)");
    activeCheck->setChecked(recurring.isActive());
    activeCheck->setCursor(Qt::PointingHandCursor);

    typeLabel = new QLabel();
    typeLabel->setObjectName("cardTitle");

    QHBoxLayout *endRow = new QHBoxLayout();
    endRow->addWidget(endCheck);
    endRow->addWidget(endDateEdit);

    formLayout->addRow("Ghi chú:", noteEdit);
    formLayout->addRow("Số tiền:", amountSpin);
    formLayout->addRow("Danh mục:", categoryCombo);
    formLayout->addRow("", typeLabel);
    formLayout->addRow("Tài khoản:", accountCombo);
    formLayout->addRow("Tần suất:", frequencyCombo);
    formLayout->addRow("Ngày kế tiếp:", nextDateEdit);
    formLayout->addRow("Kết thúc:", endRow);
    formLayout->addRow("", activeCheck);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    layout->addLayout(formLayout);
    layout->addWidget(buttonBox);
    setLayout(layout);

    connect(endCheck, &QCheckBox::toggled, endDateEdit, &QWidget::setDisabled);
    connect(categoryCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this](int) { updateTypeLabel(); });

    // Tải danh sách danh mục
    QVector<Category> categories =
        AppContext::instance().categoryService().getAllCategories();
    int selectCategory = 0;
    for (int i = 0; i < categories.size(); ++i) {
        categoryCombo->addItem(categories[i].getName(), categories[i].getId());
        if (categories[i].getId() == recurring.getCategoryId())
            selectCategory = i;
    }
    if (categoryCombo->count() > 0)
        categoryCombo->setCurrentIndex(selectCategory);

    // Tải danh sách tài khoản
    QVector<Account> accounts =
        AppContext::instance().accountService().getAllAccounts();
    int selectAccount = 0;
    for (int i = 0; i < accounts.size(); ++i) {
        accountCombo->addItem(accounts[i].getName(), accounts[i].getId());
        if (accounts[i].getId() == recurring.getAccountId())
            selectAccount = i;
    }
    if (accountCombo->count() > 0)
        accountCombo->setCurrentIndex(selectAccount);

    // Chu kỳ
    int freqIndex = frequencyCombo->findText(recurring.frequencyDisplay());
    frequencyCombo->setCurrentIndex(freqIndex < 0 ? 0 : freqIndex);

    updateTypeLabel();
}

void RecurringTransactionDialog::updateTypeLabel()
{
    int catId = categoryCombo->currentData().toInt();
    QVector<Category> categories =
        AppContext::instance().categoryService().getAllCategories();
    for (const Category &cat : categories) {
        if (cat.getId() == catId) {
            typeLabel->setText(cat.getType() == CategoryType::Income
                                   ? "Loại: Thu Nhập"
                                   : "Loại: Chi Tiêu");
            typeLabel->setStyleSheet(
                QString("color: %1; font-size: 12px;")
                    .arg(cat.getType() == CategoryType::Income
                             ? "#059669" : "#E11D48"));
            return;
        }
    }
    typeLabel->setText("");
}

RecurringTransaction RecurringTransactionDialog::getRecurringTransaction() const
{
    // Type lấy từ category đang chọn
    QString type = "expense";
    QVector<Category> categories =
        AppContext::instance().categoryService().getAllCategories();
    for (const Category &cat : categories) {
        if (cat.getId() == categoryCombo->currentData().toInt()) {
            type = (cat.getType() == CategoryType::Income) ? "income" : "expense";
            break;
        }
    }

    // Frequency: index 0=Monthly, 1=Weekly, 2=Daily, 3=Yearly
    QString freq = "monthly";
    switch (frequencyCombo->currentIndex()) {
    case 1: freq = "weekly"; break;
    case 2: freq = "daily"; break;
    case 3: freq = "yearly"; break;
    default: freq = "monthly"; break;
    }

    return RecurringTransaction(
        m_id,
        noteEdit->text().trimmed(),
        amountSpin->value(),
        type,
        accountCombo->currentData().toInt(),
        categoryCombo->currentData().toInt(),
        freq,
        nextDateEdit->date(),
        endCheck->isChecked() ? QDate() : endDateEdit->date(),
        activeCheck->isChecked());
}
