// ============================================
// File: src/ui/dialogs/TransactionDialog.cpp
// Nguoi viet: Viet Tuong (GUI Developer)
// Mo ta: Dialog them/sua giao dich —
//        Form day du voi so tien, danh muc, ngay,
//        loai (thu/chi), ghi chu.
//        Ho tro ca 2 che do: them moi va chinh sua
// ============================================
#include "TransactionDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDate>

#include "core/repositories/CategoryRepository.h"

// ==================== CONSTRUCTOR: THEM MOI ====================
TransactionDialog::TransactionDialog(QWidget *parent)
    : QDialog(parent)
    , m_editId(0)
    , m_editAccountId(1)  // Mac dinh account ID = 1
{
    setupUI();
    loadCategories();
    m_dateEdit->setDate(QDate::currentDate());
}

// ==================== CONSTRUCTOR: SUA ====================
TransactionDialog::TransactionDialog(const Transaction &transaction,
                                       QWidget *parent)
    : QDialog(parent)
    , m_editId(transaction.getId())
    , m_editAccountId(transaction.getAccountId())
{
    setupUI();
    loadCategories();
    populateForm(transaction);
}

// ==================== SETUP GIAO DIEN ====================
void TransactionDialog::setupUI()
{
    setMinimumWidth(420);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(15);

    // Tieu de
    QLabel *title = new QLabel(
        m_editId > 0 ? "Sua giao dich" : "Them giao dich moi");
    title->setStyleSheet(
        "font-size: 18px; font-weight: bold; color: #1A237E;");
    mainLayout->addWidget(title);

    // === FORM NHAP LIEU ===
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(12);
    formLayout->setLabelAlignment(Qt::AlignRight);

    // So tien
    m_amountEdit = new QLineEdit();
    m_amountEdit->setPlaceholderText("VD: 50000");
    formLayout->addRow("So tien (VND):", m_amountEdit);

    // Danh muc (load tu CategoryRepository)
    m_categoryCombo = new QComboBox();
    formLayout->addRow("Danh muc:", m_categoryCombo);

    // Ngay giao dich
    m_dateEdit = new QDateEdit();
    m_dateEdit->setCalendarPopup(true);
    m_dateEdit->setDisplayFormat("dd/MM/yyyy");
    formLayout->addRow("Ngay:", m_dateEdit);

    // Loai giao dich
    m_typeCombo = new QComboBox();
    m_typeCombo->addItem("Chi tieu", "expense");
    m_typeCombo->addItem("Thu nhap", "income");
    formLayout->addRow("Loai:", m_typeCombo);

    // Ghi chu
    m_noteEdit = new QLineEdit();
    m_noteEdit->setPlaceholderText("Ghi chu (tuy chon)");
    formLayout->addRow("Ghi chu:", m_noteEdit);

    mainLayout->addLayout(formLayout);

    // === NUT HANH DONG ===
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    QPushButton *cancelBtn = new QPushButton("Huy");
    cancelBtn->setCursor(Qt::PointingHandCursor);
    cancelBtn->setStyleSheet(
        "QPushButton { background: #EEEEEE; color: #424242; border: none; "
        "border-radius: 6px; padding: 9px 22px; font-size: 13px; }"
        "QPushButton:hover { background: #E0E0E0; }");
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    QPushButton *saveBtn = new QPushButton("Luu");
    saveBtn->setObjectName("primaryBtn");
    saveBtn->setCursor(Qt::PointingHandCursor);
    connect(saveBtn, &QPushButton::clicked,
            this, &TransactionDialog::onSaveClicked);

    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(saveBtn);
    mainLayout->addLayout(btnLayout);
}

// ==================== LOAD DANH MUC TU DB ====================
void TransactionDialog::loadCategories()
{
    CategoryRepository catRepo;
    m_categories = catRepo.getAllCategories();

    m_categoryCombo->clear();
    for (const Category &cat : m_categories) {
        // Luu ID vao itemData de truy xuat sau nay
        m_categoryCombo->addItem(cat.getName(), cat.getId());
    }
}

// ==================== DIEN FORM KHI SUA ====================
void TransactionDialog::populateForm(const Transaction &transaction)
{
    m_amountEdit->setText(
        QString::number(transaction.getAmount(), 'f', 0));

    // Chon dung danh muc trong combo box
    for (int i = 0; i < m_categoryCombo->count(); ++i) {
        if (m_categoryCombo->itemData(i).toInt() == transaction.getCategoryId()) {
            m_categoryCombo->setCurrentIndex(i);
            break;
        }
    }

    m_dateEdit->setDate(transaction.getDate());

    // Chon dung loai giao dich
    int typeIndex = (transaction.getType() == "income") ? 1 : 0;
    m_typeCombo->setCurrentIndex(typeIndex);

    m_noteEdit->setText(transaction.getNote());
}

// ==================== XU LY NUT LUU ====================
void TransactionDialog::onSaveClicked()
{
    // Kiem tra so tien hop le
    bool ok;
    double amount = m_amountEdit->text().toDouble(&ok);
    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "Loi",
            "So tien phai la so duong hop le!");
        return;
    }

    // Kiem tra da chon danh muc chua
    if (m_categoryCombo->count() == 0) {
        QMessageBox::warning(this, "Loi",
            "Vui long them danh muc truoc khi tao giao dich!");
        return;
    }

    accept();
}

// ==================== LAY KET QUA ====================
Transaction TransactionDialog::getTransaction() const
{
    double amount    = m_amountEdit->text().toDouble();
    int categoryId   = m_categoryCombo->currentData().toInt();
    QDate date       = m_dateEdit->date();
    QString type     = m_typeCombo->currentData().toString();
    QString note     = m_noteEdit->text();

    return Transaction(m_editId, m_editAccountId, categoryId,
                        amount, date, note, type);
}
