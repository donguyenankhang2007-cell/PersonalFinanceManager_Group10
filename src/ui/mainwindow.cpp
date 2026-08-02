// ============================================
// File: src/ui/mainwindow.cpp
// Mo ta: Cua so chinh voi sidebar icon (thu gon),
//        hover hien tooltip ten tab, avatar o goc trai duoi,
//        va tab Settings. Toan bo giao dien dung ThemeManager.
// ============================================
#include "mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QIcon>
#include <QEvent>

#include "theme/ThemeManager.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setObjectName("centralRoot");
    setWindowTitle("Personal Finance Manager — Group 10");
    resize(1180, 760);
    setMinimumSize(980, 640);

    setupPages();
    setupConnections();

    // Áp theme (theo cài đặt đã lưu trong QSettings)
    ThemeManager::instance().applyTo(this);
}

MainWindow::~MainWindow()
{
}

void MainWindow::buildNavButton(QPushButton *btn, const QString &icon,
                                const QString &tooltip)
{
    btn->setObjectName("navButton");
    btn->setIcon(QIcon(icon));
    btn->setIconSize(QSize(20, 20));
    btn->setFixedSize(52, 46);
    btn->setCursor(Qt::PointingHandCursor);
    btn->setCheckable(true);
    btn->setProperty("tooltipText", tooltip);
    btn->installEventFilter(this);
}

// ==================== SETUP PAGES ====================
void MainWindow::setupPages()
{
    QWidget *central = new QWidget(this);
    central->setObjectName("centralRoot");
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // === SIDEBAR (thu gọn, chỉ icon) ===
    QWidget *sidebar = new QWidget();
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(76);

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(12, 16, 12, 16);
    sidebarLayout->setSpacing(6);

    // Logo (now replaced by avatar on top)
    avatarButton = new QPushButton();
    avatarButton->setObjectName("avatarButton");
    avatarButton->setIcon(QIcon(ThemeManager::instance().avatarPath()));
    avatarButton->setIconSize(QSize(22, 22));
    avatarButton->setFixedSize(44, 44);
    avatarButton->setCursor(Qt::PointingHandCursor);
    avatarButton->setProperty("tooltipText", "Quản Lý Tài Khoản");
    avatarButton->installEventFilter(this);

    // Navigation buttons (icon + tooltip khi hover)
    btnDashboard   = new QPushButton();
    btnTransaction = new QPushButton();
    btnCategory    = new QPushButton();
    btnBudget      = new QPushButton();
    btnRecurring   = new QPushButton();
    btnSettings    = new QPushButton();

    buildNavButton(btnDashboard,   ":/icons/dashboard.svg",   "Trang Chủ");
    buildNavButton(btnTransaction, ":/icons/transaction.svg", "Giao Dịch");
    buildNavButton(btnCategory,    ":/icons/category.svg",    "Danh Mục");
    buildNavButton(btnBudget,      ":/icons/budget.svg",      "Ngân Sách");
    buildNavButton(btnRecurring,   ":/icons/repeat.svg",      "Giao Dịch Định Kỳ");
    buildNavButton(btnSettings,    ":/icons/settings.svg",    "Cài Đặt");

    sidebarLayout->addWidget(avatarButton, 0, Qt::AlignHCenter);
    sidebarLayout->addSpacing(24);
    sidebarLayout->addWidget(btnDashboard, 0, Qt::AlignHCenter);
    sidebarLayout->addWidget(btnTransaction, 0, Qt::AlignHCenter);
    sidebarLayout->addWidget(btnCategory, 0, Qt::AlignHCenter);
    sidebarLayout->addWidget(btnBudget, 0, Qt::AlignHCenter);
    sidebarLayout->addWidget(btnRecurring, 0, Qt::AlignHCenter);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(btnSettings, 0, Qt::AlignHCenter);

    // === Custom Hover Tooltip ===
    hoverTooltip = new QLabel(this);
    hoverTooltip->setObjectName("hoverTooltip");
    hoverTooltip->setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint);
    hoverTooltip->setAttribute(Qt::WA_ShowWithoutActivating);
    hoverTooltip->setStyleSheet("background-color: #1E293B; color: #F1F5F9; border-radius: 6px; padding: 6px 12px; font-size: 12px; font-weight: bold;");
    hoverTooltip->hide();

    // === CONTENT AREA ===
    stackedWidget = new QStackedWidget();
    stackedWidget->setObjectName("contentArea");

    dashboard    = new DashboardPage();
    accountPage  = new AccountPage();
    category     = new CategoryPage();
    transaction  = new TransactionPage();
    budget       = new BudgetPage();
    recurringPage = new RecurringPage();
    settingsPage = new SettingsPage();

    stackedWidget->addWidget(dashboard);      // 0
    stackedWidget->addWidget(accountPage);    // 1
    stackedWidget->addWidget(category);       // 2
    stackedWidget->addWidget(transaction);    // 3
    stackedWidget->addWidget(budget);         // 4
    stackedWidget->addWidget(recurringPage);  // 5
    stackedWidget->addWidget(settingsPage);   // 6

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stackedWidget);

    setCentralWidget(central);

    // Mặc định mở Dashboard
    stackedWidget->setCurrentIndex(DashboardIndex);
    syncNav(DashboardIndex);
    dashboard->loadData();
}

// ==================== CONNECTIONS ====================
void MainWindow::setupConnections()
{
    connect(btnDashboard, &QPushButton::clicked,
            this, [this]() { showPage(DashboardIndex); });
    connect(btnTransaction, &QPushButton::clicked,
            this, [this]() { showPage(TransactionIndex); });
    connect(btnCategory, &QPushButton::clicked,
            this, [this]() { showPage(CategoryIndex); });
    connect(btnBudget, &QPushButton::clicked,
            this, [this]() { showPage(BudgetIndex); });
    connect(btnRecurring, &QPushButton::clicked,
            this, [this]() { showPage(RecurringIndex); });
    connect(btnSettings, &QPushButton::clicked,
            this, [this]() { showPage(SettingsIndex); });

    // Avatar mở trang Account Management
    connect(avatarButton, &QPushButton::clicked,
            this, [this]() { showPage(AccountIndex); });

    // Khi đổi theme (accent/density/charts) → áp lại stylesheet
    ThemeManager &theme = ThemeManager::instance();
    connect(&theme, &ThemeManager::themeChanged, this, [this]() {
        ThemeManager::instance().applyTo(this);
    });

    connect(&theme, &ThemeManager::avatarChanged, this, [this](const QString &path) {
        avatarButton->setIcon(QIcon(path));
    });

    // Khi đổi mật độ bảng → làm mới trang hiện tại để áp chiều cao hàng
    connect(&theme, &ThemeManager::densityChanged, this, [this]() {
        int idx = stackedWidget->currentIndex();
        showPage(idx);
    });
}

// ==================== NAVIGATION ====================
void MainWindow::syncNav(int activeIndex)
{
    QPushButton* btns[] = {btnDashboard, btnCategory, btnTransaction,
                           btnBudget, btnRecurring, btnSettings};
    int pageIndex[] = {DashboardIndex, CategoryIndex, TransactionIndex,
                       BudgetIndex, RecurringIndex, SettingsIndex};
    // Trang Accounts truy cập qua avatar, không nằm trong nav
    for (int i = 0; i < 6; ++i) {
        if (btns[i]) btns[i]->setChecked(pageIndex[i] == activeIndex);
    }
}

void MainWindow::showPage(int index)
{
    switch (index) {
    case DashboardIndex:
        dashboard->loadData();
        break;
    case AccountIndex:
        accountPage->loadAccounts();
        break;
    case CategoryIndex:
        category->loadCategories();
        break;
    case TransactionIndex:
        transaction->loadTransactions();
        break;
    case BudgetIndex:
        budget->loadBudgets();
        break;
    case RecurringIndex:
        recurringPage->loadRecurring();
        break;
    default:
        break;
    }

    stackedWidget->setCurrentIndex(index);
    syncNav(index);
}

// ==================== EVENT FILTER (Hover Tooltip) ====================
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::HoverEnter || event->type() == QEvent::Enter) {
        QPushButton* btn = qobject_cast<QPushButton*>(watched);
        if (btn && btn->property("tooltipText").isValid()) {
            hoverTooltip->setText(btn->property("tooltipText").toString());
            hoverTooltip->adjustSize();
            QPoint globalPos = btn->mapToGlobal(QPoint(btn->width() + 12, (btn->height() - hoverTooltip->height()) / 2));
            hoverTooltip->move(globalPos);
            hoverTooltip->show();
        }
    } else if (event->type() == QEvent::HoverLeave || event->type() == QEvent::Leave) {
        QPushButton* btn = qobject_cast<QPushButton*>(watched);
        if (btn && btn->property("tooltipText").isValid()) {
            hoverTooltip->hide();
        }
    }
    return QMainWindow::eventFilter(watched, event);
}
