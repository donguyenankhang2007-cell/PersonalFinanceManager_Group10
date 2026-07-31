// ============================================
// File: src/ui/dialogs/CategoryDialog.cpp
// Nguoi viet: Viet Tuong (GUI Developer)
// Mo ta: Dialog them/sua danh muc —
//        Form voi ten, loai (thu/chi), ma mau, icon
//        Ho tro ca 2 che do: them moi va chinh sua
// ============================================
#include "CategoryDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>

// ==================== CONSTRUCTOR: THEM MOI ====================
CategoryDialog::CategoryDialog(QWidget *parent)
    : QDialog(parent)
    , m_editId(0)
{
    setupUI();
}

// ==================== CONSTRUCTOR: SUA ====================
CategoryDialog::CategoryDialog(const Category &category, QWidget *parent)
    : QDialog(parent)
    , m_editId(category.getId())
{
    setupUI();
    populateForm(category);
}

// ==================== SETUP GIAO DIEN ====================
void CategoryDialog::setupUI()
{
    setMinimumWidth(380);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(15);

    // Tieu de
    QLabel *title = new QLabel(
        m_editId > 0 ? "Sua danh muc" : "Them danh muc moi");
    title->setStyleSheet(
        "font-size: 18px; font-weight: bold; color: #1A237E;");
    mainLayout->addWidget(title);

    // === FORM NHAP LIEU ===
    QFormLayout *formLayout = new QFormLayout();
    formLayout->setSpacing(12);
    formLayout->setLabelAlignment(Qt::AlignRight);

    // Ten danh muc
    m_nameEdit = new QLineEdit();
    m_nameEdit->setPlaceholderText("VD: An uong, Di chuyen, Luong...");
    formLayout->addRow("Ten danh muc:", m_nameEdit);

    // Loai (Thu nhap / Chi tieu)
    m_typeCombo = new QComboBox();
    m_typeCombo->addItem("Chi tieu",
        static_cast<int>(CategoryType::Expense));
    m_typeCombo->addItem("Thu nhap",
        static_cast<int>(CategoryType::Income));
    formLayout->addRow("Loai:", m_typeCombo);

    // Ma mau
    m_colorEdit = new QLineEdit();
    m_colorEdit->setPlaceholderText("VD: #FF5722, #4CAF50...");
    formLayout->addRow("Ma mau:", m_colorEdit);

    // Icon
    m_iconEdit = new QLineEdit();
    m_iconEdit->setPlaceholderText("VD: food, transport, salary...");
    formLayout->addRow("Icon:", m_iconEdit);

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
            this, &CategoryDialog::onSaveClicked);

    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(saveBtn);
    mainLayout->addLayout(btnLayout);
}

// ==================== DIEN FORM KHI SUA ====================
void CategoryDialog::populateForm(const Category &category)
{
    m_nameEdit->setText(category.getName());

    int typeIndex = (category.getType() == CategoryType::Income) ? 1 : 0;
    m_typeCombo->setCurrentIndex(typeIndex);

    m_colorEdit->setText(category.getColor());
    m_iconEdit->setText(category.getIcon());
}

// ==================== XU LY NUT LUU ====================
void CategoryDialog::onSaveClicked()
{
    if (m_nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Loi",
            "Vui long nhap ten danh muc!");
        return;
    }
    accept();
}

// ==================== LAY KET QUA ====================
Category CategoryDialog::getCategory() const
{
    CategoryType type = static_cast<CategoryType>(
        m_typeCombo->currentData().toInt());

    return Category(
        m_editId,
        m_nameEdit->text().trimmed(),
        type,
        m_colorEdit->text().trimmed(),
        m_iconEdit->text().trimmed()
    );
}
