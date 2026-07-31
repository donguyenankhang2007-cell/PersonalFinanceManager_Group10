// ============================================
// File: src/ui/pages/BudgetPage.cpp
// Nguoi viet: Viet Tuong (GUI Developer)
// Mo ta: Trang ngan sach — hien thi tien do
//        chi tieu theo tung danh muc bang progress bar
//        Ket noi BudgetRepository, BudgetService
// ============================================
#include "BudgetPage.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QFrame>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QDate>
#include <QMessageBox>
#include <QDialog>

#include "core/repositories/BudgetRepository.h"
#include "core/repositories/CategoryRepository.h"
#include "core/repositories/TransactionRepository.h"
#include "core/services/BudgetService.h"
#include "utils/MoneyUtils.h"

BudgetPage::BudgetPage(QWidget *parent)
    : QWidget(parent)
{
    setupUI();
}

// ==================== SETUP GIAO DIEN ====================
void BudgetPage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(25, 25, 25, 25);
    mainLayout->setSpacing(15);

    // Tieu de
    QLabel *title = new QLabel("Ngan sach hang thang");
    title->setStyleSheet("color: #1A237E; font-size: 22px; font-weight: bold;");
    QLabel *subtitle = new QLabel("Theo doi chi tieu theo tung danh muc");
    subtitle->setStyleSheet("color: #757575; font-size: 13px;");
    mainLayout->addWidget(title);
    mainLayout->addWidget(subtitle);

    // Nut them ngan sach
    QHBoxLayout *actionLayout = new QHBoxLayout();
    m_btnAdd = new QPushButton("+ Them ngan sach");
    m_btnAdd->setObjectName("primaryBtn");
    m_btnAdd->setCursor(Qt::PointingHandCursor);
    actionLayout->addWidget(m_btnAdd);
    actionLayout->addStretch();
    mainLayout->addLayout(actionLayout);

    // Vung cuon chua cac the ngan sach
    m_scrollArea = new QScrollArea();
    m_scrollArea->setWidgetResizable(true);

    QWidget *scrollContent = new QWidget();
    scrollContent->setStyleSheet("background: transparent;");
    m_cardsLayout = new QVBoxLayout(scrollContent);
    m_cardsLayout->setSpacing(12);
    m_cardsLayout->addStretch();

    m_scrollArea->setWidget(scrollContent);
    mainLayout->addWidget(m_scrollArea, 1);

    connect(m_btnAdd, &QPushButton::clicked, this, &BudgetPage::onAddClicked);
}

// ==================== TAO THE NGAN SACH ====================
QWidget* BudgetPage::createBudgetCard(const QString &categoryName,
                                        double budgetAmount, double spentAmount)
{
    QFrame *card = new QFrame();
    card->setStyleSheet(
        "QFrame { background: white; border: 1px solid #E8EAF6; "
        "border-radius: 10px; padding: 16px; }");

    QVBoxLayout *layout = new QVBoxLayout(card);
    layout->setSpacing(10);

    // Header: ten danh muc + so tien
    QHBoxLayout *headerLayout = new QHBoxLayout();

    QLabel *nameLabel = new QLabel(categoryName);
    nameLabel->setStyleSheet(
        "font-size: 15px; font-weight: bold; color: #1A237E;");

    double percent = (budgetAmount > 0)
        ? (spentAmount / budgetAmount * 100.0) : 0;

    QLabel *amountLabel = new QLabel(
        QString("%1 / %2")
            .arg(MoneyUtils::formatVND(spentAmount))
            .arg(MoneyUtils::formatVND(budgetAmount)));
    amountLabel->setStyleSheet("font-size: 12px; color: #757575;");
    amountLabel->setAlignment(Qt::AlignRight);

    headerLayout->addWidget(nameLabel);
    headerLayout->addWidget(amountLabel);
    layout->addLayout(headerLayout);

    // Thanh tien do (Progress bar)
    QProgressBar *progress = new QProgressBar();
    progress->setRange(0, 100);
    progress->setValue(qMin(static_cast<int>(percent), 100));
    progress->setFormat(QString::number(percent, 'f', 1) + "%");

    // Mau thanh thay doi theo muc su dung:
    // Xanh duong (< 70%) → Cam (70-90%) → Do (> 90%)
    QString chunkColor;
    if (percent > 90) {
        chunkColor = "#E53935";  // Do — vuot ngan sach
    } else if (percent > 70) {
        chunkColor = "#FF9800";  // Cam — canh bao
    } else {
        chunkColor = "#1976D2";  // Xanh — binh thuong
    }

    progress->setStyleSheet(QString(
        "QProgressBar { border: 1px solid #E0E0E0; border-radius: 8px; "
        "background: #F5F5F5; height: 24px; text-align: center; "
        "font-size: 12px; font-weight: bold; }"
        "QProgressBar::chunk { border-radius: 7px; background: %1; }"
    ).arg(chunkColor));

    layout->addWidget(progress);

    // Canh bao neu vuot ngan sach
    if (percent > 100) {
        QLabel *warning = new QLabel(
            QString("Vuot ngan sach %1!")
                .arg(MoneyUtils::formatVND(spentAmount - budgetAmount)));
        warning->setStyleSheet(
            "color: #E53935; font-size: 12px; font-weight: bold;");
        layout->addWidget(warning);
    }

    return card;
}

// ==================== TAI DU LIEU ====================
void BudgetPage::loadData()
{
    // Xoa cac the cu (giu lai stretch o cuoi)
    while (m_cardsLayout->count() > 1) {
        QLayoutItem *item = m_cardsLayout->takeAt(0);
        if (item->widget()) {
            delete item->widget();
        }
        delete item;
    }

    BudgetRepository budgetRepo;
    CategoryRepository catRepo;
    TransactionRepository transRepo;
    BudgetService budgetService;

    QVector<Budget> budgets = budgetRepo.getAllBudgets();
    QVector<Category> categories = catRepo.getAllCategories();
    QVector<Transaction> transactions = transRepo.getAllTransactions();

    // Bang tra cuu ten danh muc
    QMap<int, QString> categoryNames;
    for (const Category &cat : categories) {
        categoryNames[cat.getId()] = cat.getName();
    }

    // Chuyen sang QList cho BudgetService
    QList<Transaction> txList(transactions.begin(), transactions.end());

    if (budgets.isEmpty()) {
        QLabel *emptyLabel = new QLabel(
            "Chua co ngan sach nao.\nBam '+ Them ngan sach' de bat dau thiet lap.");
        emptyLabel->setStyleSheet(
            "color: #757575; font-size: 14px; padding: 30px;");
        emptyLabel->setAlignment(Qt::AlignCenter);
        m_cardsLayout->insertWidget(0, emptyLabel);
        return;
    }

    // Tao the cho tung budget
    for (const Budget &budget : budgets) {
        QString catName = categoryNames.value(
            budget.getCategoryId(), "Khong ro");
        double spent = budgetService.getSpentAmount(
            budget, budget.getCategoryId(), txList);

        QWidget *card = createBudgetCard(catName, budget.getAmount(), spent);
        // Chen truoc stretch
        m_cardsLayout->insertWidget(m_cardsLayout->count() - 1, card);
    }
}

// ==================== THEM NGAN SACH (INLINE DIALOG) ====================
void BudgetPage::onAddClicked()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Them ngan sach moi");
    dialog.setMinimumWidth(380);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);
    layout->setContentsMargins(25, 25, 25, 25);
    layout->setSpacing(12);

    // Tieu de
    QLabel *dlgTitle = new QLabel("Them ngan sach moi");
    dlgTitle->setStyleSheet(
        "font-size: 18px; font-weight: bold; color: #1A237E;");
    layout->addWidget(dlgTitle);

    // Chon danh muc
    QLabel *catLabel = new QLabel("Danh muc:");
    catLabel->setStyleSheet("font-weight: bold; color: #424242;");
    QComboBox *catCombo = new QComboBox();
    CategoryRepository catRepo;
    QVector<Category> categories = catRepo.getAllCategories();
    for (const Category &cat : categories) {
        catCombo->addItem(cat.getName(), cat.getId());
    }
    layout->addWidget(catLabel);
    layout->addWidget(catCombo);

    // So tien han muc
    QLabel *amountLabel = new QLabel("Han muc ngan sach (VND):");
    amountLabel->setStyleSheet("font-weight: bold; color: #424242;");
    QLineEdit *amountEdit = new QLineEdit();
    amountEdit->setPlaceholderText("VD: 5000000");
    layout->addWidget(amountLabel);
    layout->addWidget(amountEdit);

    // Thang
    QLabel *monthLabel = new QLabel("Thang:");
    monthLabel->setStyleSheet("font-weight: bold; color: #424242;");
    QSpinBox *monthSpin = new QSpinBox();
    monthSpin->setRange(1, 12);
    monthSpin->setValue(QDate::currentDate().month());
    layout->addWidget(monthLabel);
    layout->addWidget(monthSpin);

    // Nam
    QLabel *yearLabel = new QLabel("Nam:");
    yearLabel->setStyleSheet("font-weight: bold; color: #424242;");
    QSpinBox *yearSpin = new QSpinBox();
    yearSpin->setRange(2020, 2030);
    yearSpin->setValue(QDate::currentDate().year());
    layout->addWidget(yearLabel);
    layout->addWidget(yearSpin);

    // Nut Luu / Huy
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    QPushButton *cancelBtn = new QPushButton("Huy");
    cancelBtn->setStyleSheet(
        "QPushButton { background: #EEEEEE; color: #424242; border: none; "
        "border-radius: 6px; padding: 8px 20px; font-size: 13px; }"
        "QPushButton:hover { background: #E0E0E0; }");
    connect(cancelBtn, &QPushButton::clicked, &dialog, &QDialog::reject);

    QPushButton *saveBtn = new QPushButton("Luu");
    saveBtn->setObjectName("primaryBtn");
    connect(saveBtn, &QPushButton::clicked, &dialog, &QDialog::accept);

    btnLayout->addWidget(cancelBtn);
    btnLayout->addWidget(saveBtn);
    layout->addLayout(btnLayout);

    if (dialog.exec() == QDialog::Accepted) {
        bool ok;
        double amount = amountEdit->text().toDouble(&ok);
        if (!ok || amount <= 0) {
            QMessageBox::warning(this, "Loi", "So tien khong hop le!");
            return;
        }

        int categoryId = catCombo->currentData().toInt();
        int month = monthSpin->value();
        int year  = yearSpin->value();

        Budget newBudget(0, categoryId, amount, month, year);
        BudgetRepository budgetRepo;
        budgetRepo.addBudget(newBudget);

        loadData();
    }
}