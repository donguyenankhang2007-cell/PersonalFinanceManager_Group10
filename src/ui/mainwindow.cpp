// ============================================
// File: src/ui/mainwindow.cpp
// Nguoi viet: Viet Tuong (GUI Developer)
// Mo ta: Cua so chinh voi sidebar navigation
//        va QStackedWidget chuyen trang
// ============================================
#include "mainwindow.h"
#include "pages/DashboardPage.h"
#include "pages/TransactionPage.h"
#include "pages/CategoryPage.h"
#include "pages/BudgetPage.h"
#include "pages/ReportPage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_currentIndex(0)
    , m_dashboardPage(nullptr)
    , m_transactionPage(nullptr)
    , m_categoryPage(nullptr)
    , m_budgetPage(nullptr)
    , m_reportPage(nullptr)
{
    setStyleSheet(getGlobalStylesheet());
    setupUI();
    setWindowTitle("Personal Finance Manager — Group 10");
    resize(1100, 700);
    setMinimumSize(900, 600);
}

MainWindow::~MainWindow() {}

// ==================== SETUP UI ====================
void MainWindow::setupUI()
{
    ui->setupUi(this);

    setWindowTitle("Personal Finance Manager");
    resize(1050, 680);

    setupPages();
    setupConnections();
}

// ==================== PAGES ====================
void MainWindow::createPages()
{
    m_dashboardPage   = new DashboardPage();
    m_transactionPage = new TransactionPage();
    m_categoryPage    = new CategoryPage();
    m_budgetPage      = new BudgetPage();
    m_reportPage      = new ReportPage();

    m_stackedWidget->addWidget(m_dashboardPage);
    m_stackedWidget->addWidget(m_transactionPage);
    m_stackedWidget->addWidget(m_categoryPage);
    m_stackedWidget->addWidget(m_budgetPage);
    m_stackedWidget->addWidget(m_reportPage);
}

// ==================== NAVIGATION ====================
void MainWindow::onNavButtonClicked(int index)
{
    setActivePage(index);
}

void MainWindow::setActivePage(int index)
{
    m_currentIndex = index;
    m_stackedWidget->setCurrentIndex(index);

    // Cap nhat trang thai cac nut (checked/unchecked)
    for (int i = 0; i < m_navButtons.size(); ++i) {
        m_navButtons[i]->setChecked(i == index);
    }

    // Lam moi du lieu khi chuyen trang
    switch (index) {
    case 0: m_dashboardPage->refreshData(); break;
    case 1: m_transactionPage->loadData(); break;
    case 2: m_categoryPage->loadData(); break;
    case 3: m_budgetPage->loadData(); break;
    case 4: m_reportPage->generateReport(); break;
    }
}

// ==================== GLOBAL STYLESHEET (BLUE THEME) ====================
QString MainWindow::getGlobalStylesheet() const
{
    return R"(

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

        /* ===== PROGRESS BARS ===== */
        QProgressBar {
            border: 1px solid #E0E0E0;
            border-radius: 8px;
            text-align: center;
            background: #F5F5F5;
            height: 22px;
            font-size: 12px;
        }
        QProgressBar::chunk {
            border-radius: 7px;
            background: qlineargradient(x1:0, y1:0, x2:1, y2:0,
                stop:0 #1976D2, stop:1 #42A5F5);
        }

        /* ===== INPUTS ===== */
        QLineEdit, QComboBox, QDateEdit, QSpinBox {
            border: 1px solid #BDBDBD;
            border-radius: 6px;
            padding: 8px 12px;
            font-size: 13px;
            background: white;
        }
        QLineEdit:focus, QComboBox:focus, QDateEdit:focus, QSpinBox:focus {
            border: 2px solid #1976D2;
        }
        QComboBox::drop-down {
            border: none;
            padding-right: 10px;
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
    )";
}

void MainWindow::setupPages()
{
    // Widget trung tâm
    QWidget *centralWidget = new QWidget(this);
    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // === Sidebar (thanh điều hướng bên trái) ===
    QWidget *sidebar = new QWidget();
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(200);
    sidebar->setStyleSheet(
        "#sidebar { background-color: #1e272e; }"
        "#sidebar QPushButton {"
        "   color: #d2dae2;"
        "   background-color: transparent;"
        "   border: none;"
        "   padding: 14px 20px;"
        "   text-align: left;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "}"
        "#sidebar QPushButton:hover {"
        "   background-color: #485460;"
        "   color: #ffffff;"
        "}"
        "#sidebar QPushButton:pressed {"
        "   background-color: #0be881;"
        "   color: #1e272e;"
        "}"
    );

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setContentsMargins(0, 0, 0, 0);
    sidebarLayout->setSpacing(0);

    // Tiêu đề sidebar
    QLabel *appTitle = new QLabel(" Finance Manager");
    appTitle->setStyleSheet(
        "color: #0be881;"
        "font-size: 15px;"
        "font-weight: bold;"
        "padding: 20px 16px 25px 16px;"
        "background-color: #1e272e;"
    );

    btnDashboard   = new QPushButton("   Dashboard");
    btnAccounts    = new QPushButton("   Accounts");
    btnCategory    = new QPushButton("   Categories");
    btnTransaction = new QPushButton("   Transactions");
    btnBudget      = new QPushButton("   Budget");
    btnReport      = new QPushButton("   Reports");

    sidebarLayout->addWidget(appTitle);
    sidebarLayout->addWidget(btnDashboard);
    sidebarLayout->addWidget(btnAccounts);
    sidebarLayout->addWidget(btnCategory);
    sidebarLayout->addWidget(btnTransaction);
    sidebarLayout->addWidget(btnBudget);
    sidebarLayout->addWidget(btnReport);
    sidebarLayout->addStretch();

    // === Stacked Widget (vùng nội dung chính) ===
    stackedWidget = new QStackedWidget();
    stackedWidget->setStyleSheet(
        "QStackedWidget { background-color: #f5f6fa; }"
        "QWidget { background-color: #f5f6fa; color: #2f3640; }"
        "QLabel { color: #2f3640; font-size: 13px; }"
        "QPushButton {"
        "   background-color: #0be881;"
        "   color: #1e272e;"
        "   border: none;"
        "   padding: 10px 20px;"
        "   font-size: 13px;"
        "   font-weight: bold;"
        "   border-radius: 5px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #05c46b;"
        "}"
        "QListWidget {"
        "   background-color: #ffffff;"
        "   color: #2f3640;"
        "   border: 1px solid #dcdde1;"
        "   border-radius: 5px;"
        "   font-size: 13px;"
        "   padding: 5px;"
        "}"
        "QListWidget::item {"
        "   padding: 8px;"
        "   border-bottom: 1px solid #f1f2f6;"
        "}"
        "QListWidget::item:selected {"
        "   background-color: #0be881;"
        "   color: #1e272e;"
        "}"
        "QTableWidget {"
        "   background-color: #ffffff;"
        "   color: #2f3640;"
        "   border: 1px solid #dcdde1;"
        "   border-radius: 5px;"
        "   gridline-color: #f1f2f6;"
        "   font-size: 13px;"
        "}"
        "QHeaderView::section {"
        "   background-color: #1e272e;"
        "   color: #ffffff;"
        "   padding: 8px;"
        "   border: none;"
        "   font-weight: bold;"
        "   font-size: 13px;"
        "}"
        "QProgressBar {"
        "   background-color: #dcdde1;"
        "   border: none;"
        "   border-radius: 8px;"
        "   height: 18px;"
        "   text-align: center;"
        "   color: #2f3640;"
        "   font-size: 11px;"
        "}"
        "QProgressBar::chunk {"
        "   background-color: #0be881;"
        "   border-radius: 8px;"
        "}"
        "QComboBox, QSpinBox, QDoubleSpinBox, QLineEdit, QDateEdit {"
        "   background-color: #ffffff;"
        "   color: #2f3640;"
        "   border: 1px solid #dcdde1;"
        "   border-radius: 4px;"
        "   padding: 6px 10px;"
        "   font-size: 13px;"
        "}"
        "QComboBox:hover, QSpinBox:hover, QDoubleSpinBox:hover,"
        "QLineEdit:hover, QDateEdit:hover {"
        "   border: 1px solid #0be881;"
        "}"
        "QComboBox::drop-down {"
        "   border: none;"
        "}"
        "QGroupBox {"
        "   font-weight: bold;"
        "   font-size: 14px;"
        "   color: #1e272e;"
        "   background-color: #ffffff;"
        "   border: 1px solid #dcdde1;"
        "   border-radius: 8px;"
        "   padding-top: 20px;"
        "   margin-top: 10px;"
        "}"
        "QGroupBox::title {"
        "   subcontrol-origin: margin;"
        "   left: 15px;"
        "   padding: 0 5px;"
        "}"
        "QScrollArea {"
        "   border: none;"
        "   background-color: transparent;"
        "}"
    );

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

    // Thêm sidebar và stacked widget vào layout chính
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(stackedWidget);

    setCentralWidget(centralWidget);

    // Hiển thị Dashboard mặc định
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::setupConnections()
{
    connect(btnDashboard, &QPushButton::clicked, this, &MainWindow::showDashboard);
    connect(btnAccounts, &QPushButton::clicked, this, &MainWindow::showAccounts);
    connect(btnCategory, &QPushButton::clicked, this, &MainWindow::showCategory);
    connect(btnTransaction, &QPushButton::clicked, this, &MainWindow::showTransaction);
    connect(btnBudget, &QPushButton::clicked, this, &MainWindow::showBudget);
    connect(btnReport, &QPushButton::clicked, this, &MainWindow::showReport);
}

void MainWindow::showDashboard()
{
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::showAccounts()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showCategory()
{
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::showTransaction()
{
    stackedWidget->setCurrentIndex(3);
}

void MainWindow::showBudget()
{
    stackedWidget->setCurrentIndex(4);
}

void MainWindow::showReport()
{
    stackedWidget->setCurrentIndex(5);
}
