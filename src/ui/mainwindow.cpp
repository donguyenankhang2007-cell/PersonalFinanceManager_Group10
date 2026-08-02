// ============================================
// File: src/ui/mainwindow.cpp
// Mo ta: Cua so chinh voi sidebar navigation
//        va QStackedWidget chuyen trang
// ============================================
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Personal Finance Manager — Group 10");
    resize(1100, 700);
    setMinimumSize(900, 600);

    setupPages();
    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
}

// ==================== SETUP PAGES ====================
void MainWindow::setupPages()
{
    // Widget trung tâm
    QWidget *central = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // === SIDEBAR ===
    QWidget *sidebar = new QWidget();
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(210);

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    sidebarLayout->setSpacing(0);

    // Tiêu đề sidebar
    QLabel *appTitle = new QLabel("  Finance Manager");
    appTitle->setObjectName("sidebarTitle");

    // Đường kẻ phân cách
    QFrame *line = new QFrame();
    line->setObjectName("sidebarLine");
    line->setFrameShape(QFrame::HLine);

    // Navigation buttons
    btnDashboard   = new QPushButton("   Dashboard");
    btnAccounts    = new QPushButton("   Accounts");
    btnCategory    = new QPushButton("   Categories");
    btnTransaction = new QPushButton("   Transactions");
    btnBudget      = new QPushButton("   Budget");
    btnReport      = new QPushButton("   Reports");

    // Đặt objectName cho styling
    btnDashboard->setObjectName("navButton");
    btnAccounts->setObjectName("navButton");
    btnCategory->setObjectName("navButton");
    btnTransaction->setObjectName("navButton");
    btnBudget->setObjectName("navButton");
    btnReport->setObjectName("navButton");

    // Cho phép toggle checked state
    btnDashboard->setCheckable(true);
    btnAccounts->setCheckable(true);
    btnCategory->setCheckable(true);
    btnTransaction->setCheckable(true);
    btnBudget->setCheckable(true);
    btnReport->setCheckable(true);

    // Dashboard mặc định được chọn
    btnDashboard->setChecked(true);

    // Footer
    QLabel *footer = new QLabel("Group 10 — OOP Lab");
    footer->setObjectName("sidebarFooter");
    footer->setAlignment(Qt::AlignCenter);

    sidebarLayout->addWidget(appTitle);
    sidebarLayout->addWidget(line);
    sidebarLayout->addWidget(btnDashboard);
    sidebarLayout->addWidget(btnAccounts);
    sidebarLayout->addWidget(btnCategory);
    sidebarLayout->addWidget(btnTransaction);
    sidebarLayout->addWidget(btnBudget);
    sidebarLayout->addWidget(btnReport);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(footer);

    // === CONTENT AREA (Stacked Widget) ===
    stackedWidget = new QStackedWidget();
    stackedWidget->setObjectName("contentArea");

    dashboard   = new DashboardPage();
    accountPage = new AccountPage();
    category    = new CategoryPage();
    transaction = new TransactionPage();
    budget      = new BudgetPage();
    report      = new ReportPage();

    stackedWidget->addWidget(dashboard);     // index 0
    stackedWidget->addWidget(accountPage);   // index 1
    stackedWidget->addWidget(category);      // index 2
    stackedWidget->addWidget(transaction);   // index 3
    stackedWidget->addWidget(budget);        // index 4
    stackedWidget->addWidget(report);        // index 5

    // Layout chính
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stackedWidget);

    setCentralWidget(central);
    stackedWidget->setCurrentIndex(0);

    // === GLOBAL STYLESHEET (Blue Theme) ===
    setStyleSheet(R"(

        /* ===== SIDEBAR ===== */
        #sidebar {
            background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                stop:0 #0D47A1, stop:1 #1565C0);
        }
        #sidebarTitle {
            color: white;
            font-size: 17px;
            font-weight: bold;
            padding: 28px 15px 20px 15px;
            letter-spacing: 0.5px;
        }
        #sidebarLine {
            color: rgba(255, 255, 255, 0.15);
            margin: 0px 20px;
        }
        #navButton {
            background: transparent;
            color: rgba(255, 255, 255, 0.7);
            border: none;
            border-left: 3px solid transparent;
            text-align: left;
            padding: 12px 24px;
            font-size: 14px;
        }
        #navButton:hover {
            background: rgba(255, 255, 255, 0.08);
            color: white;
        }
        #navButton:checked {
            background: rgba(255, 255, 255, 0.15);
            color: white;
            border-left: 3px solid #64B5F6;
            font-weight: bold;
        }
        #sidebarFooter {
            color: rgba(255, 255, 255, 0.35);
            font-size: 11px;
            padding: 10px;
        }

        /* ===== CONTENT AREA ===== */
        #contentArea {
            background: #F0F2F5;
        }

        /* ===== BUTTONS ===== */
        QPushButton#primaryBtn {
            background-color: #1976D2;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 9px 22px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton#primaryBtn:hover {
            background-color: #1565C0;
        }
        QPushButton#primaryBtn:pressed {
            background-color: #0D47A1;
        }
        QPushButton#editBtn {
            background-color: #FF9800;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 9px 22px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton#editBtn:hover {
            background-color: #F57C00;
        }
        QPushButton#deleteBtn {
            background-color: #E53935;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 9px 22px;
            font-size: 13px;
            font-weight: 600;
        }
        QPushButton#deleteBtn:hover {
            background-color: #C62828;
        }

        /* ===== TABLES ===== */
        QTableWidget {
            background: white;
            border: 1px solid #E0E0E0;
            border-radius: 8px;
            gridline-color: #F0F0F0;
            font-size: 13px;
            color: #212121;
        }
        QTableWidget::item {
            padding: 8px;
        }
        QTableWidget::item:selected {
            background-color: #BBDEFB;
            color: #1A237E;
        }
        QHeaderView::section {
            background-color: #1976D2;
            color: white;
            padding: 10px 8px;
            border: none;
            font-weight: bold;
            font-size: 13px;
        }

        /* ===== LABELS ===== */
        QLabel {
            color: #212121;
            font-size: 13px;
        }

        /* ===== LIST WIDGETS ===== */
        QListWidget {
            background-color: #ffffff;
            color: #212121;
            border: 1px solid #E0E0E0;
            border-radius: 6px;
            font-size: 13px;
            padding: 5px;
        }
        QListWidget::item {
            padding: 8px;
            border-bottom: 1px solid #F0F0F0;
        }
        QListWidget::item:selected {
            background-color: #BBDEFB;
            color: #1A237E;
        }

        /* ===== PROGRESS BARS ===== */
        QProgressBar {
            border: 1px solid #E0E0E0;
            border-radius: 8px;
            text-align: center;
            background: #F5F5F5;
            height: 22px;
            font-size: 12px;
            color: #212121;
        }
        QProgressBar::chunk {
            border-radius: 7px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #1976D2, stop:1 #42A5F5);
        }

        /* ===== INPUTS ===== */
        QLineEdit, QComboBox, QDateEdit, QSpinBox, QDoubleSpinBox {
            border: 1px solid #BDBDBD;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 13px;
            background: white;
            color: #212121;
        }
        QLineEdit:focus, QComboBox:focus, QDateEdit:focus,
        QSpinBox:focus, QDoubleSpinBox:focus {
            border: 2px solid #1976D2;
        }
        QComboBox::drop-down {
            border: none;
            padding-right: 10px;
        }

        /* ===== GROUP BOX ===== */
        QGroupBox {
            font-weight: bold;
            font-size: 14px;
            color: #212121;
            background-color: #ffffff;
            border: 1px solid #E0E0E0;
            border-radius: 8px;
            padding-top: 20px;
            margin-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            left: 15px;
            padding: 0 5px;
        }

        /* ===== DIALOGS ===== */
        QDialog {
            background: white;
        }

        /* ===== SCROLL AREA ===== */
        QScrollArea {
            border: none;
            background: transparent;
        }

        /* ===== GENERAL PUSH BUTTONS (content area) ===== */
        #contentArea QPushButton {
            background-color: #1976D2;
            color: white;
            border: none;
            border-radius: 6px;
            padding: 9px 22px;
            font-size: 13px;
            font-weight: 600;
        }
        #contentArea QPushButton:hover {
            background-color: #1565C0;
        }
    )");
}

// ==================== CONNECTIONS ====================
void MainWindow::setupConnections()
{
    connect(btnDashboard, &QPushButton::clicked, this, &MainWindow::showDashboard);
    connect(btnAccounts, &QPushButton::clicked, this, &MainWindow::showAccounts);
    connect(btnCategory, &QPushButton::clicked, this, &MainWindow::showCategory);
    connect(btnTransaction, &QPushButton::clicked, this, &MainWindow::showTransaction);
    connect(btnBudget, &QPushButton::clicked, this, &MainWindow::showBudget);
    connect(btnReport, &QPushButton::clicked, this, &MainWindow::showReport);
}

// ==================== NAVIGATION SLOTS ====================
static void updateNavButtons(QPushButton* buttons[], int count, int active)
{
    for (int i = 0; i < count; ++i) {
        buttons[i]->setChecked(i == active);
    }
}

void MainWindow::showDashboard()
{
    stackedWidget->setCurrentIndex(0);
    QPushButton* btns[] = {btnDashboard, btnAccounts, btnCategory, btnTransaction, btnBudget, btnReport};
    updateNavButtons(btns, 6, 0);
}

void MainWindow::showAccounts()
{
    stackedWidget->setCurrentIndex(1);
    QPushButton* btns[] = {btnDashboard, btnAccounts, btnCategory, btnTransaction, btnBudget, btnReport};
    updateNavButtons(btns, 6, 1);
}

void MainWindow::showCategory()
{
    stackedWidget->setCurrentIndex(2);
    QPushButton* btns[] = {btnDashboard, btnAccounts, btnCategory, btnTransaction, btnBudget, btnReport};
    updateNavButtons(btns, 6, 2);
}

void MainWindow::showTransaction()
{
    stackedWidget->setCurrentIndex(3);
    QPushButton* btns[] = {btnDashboard, btnAccounts, btnCategory, btnTransaction, btnBudget, btnReport};
    updateNavButtons(btns, 6, 3);
}

void MainWindow::showBudget()
{
    stackedWidget->setCurrentIndex(4);
    QPushButton* btns[] = {btnDashboard, btnAccounts, btnCategory, btnTransaction, btnBudget, btnReport};
    updateNavButtons(btns, 6, 4);
}

void MainWindow::showReport()
{
    stackedWidget->setCurrentIndex(5);
    QPushButton* btns[] = {btnDashboard, btnAccounts, btnCategory, btnTransaction, btnBudget, btnReport};
    updateNavButtons(btns, 6, 5);
}
