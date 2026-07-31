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
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QHBoxLayout *mainLayout = new QHBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // === SIDEBAR (ben trai) ===
    QWidget *sidebar = new QWidget();
    sidebar->setObjectName("sidebar");
    sidebar->setFixedWidth(230);
    createSidebar(sidebar);
    mainLayout->addWidget(sidebar);

    // === CONTENT AREA (ben phai) ===
    m_stackedWidget = new QStackedWidget();
    m_stackedWidget->setObjectName("contentArea");
    createPages();
    mainLayout->addWidget(m_stackedWidget, 1);

    // Mac dinh hien Dashboard
    setActivePage(0);
}

// ==================== SIDEBAR ====================
void MainWindow::createSidebar(QWidget *sidebarWidget)
{
    QVBoxLayout *layout = new QVBoxLayout(sidebarWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    // Tieu de ung dung
    QLabel *titleLabel = new QLabel("Personal\nFinance Manager");
    titleLabel->setObjectName("sidebarTitle");
    titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(titleLabel);

    // Duong ke ngang
    QFrame *line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setObjectName("sidebarLine");
    layout->addWidget(line);

    layout->addSpacing(10);

    // Cac nut dieu huong
    QStringList labels = {
        "   Dashboard",
        "   Giao dich",
        "   Danh muc",
        "   Ngan sach",
        "   Bao cao"
    };

    for (int i = 0; i < labels.size(); ++i) {
        QPushButton *btn = new QPushButton(labels[i]);
        btn->setObjectName("navButton");
        btn->setCheckable(true);
        btn->setCursor(Qt::PointingHandCursor);
        btn->setMinimumHeight(48);

        // Ket noi signal/slot — khi bam nut thi chuyen trang
        connect(btn, &QPushButton::clicked, this, [this, i]() {
            onNavButtonClicked(i);
        });

        m_navButtons.append(btn);
        layout->addWidget(btn);
    }

    layout->addStretch();

    // Footer phien ban
    QLabel *footer = new QLabel("v1.0 — OOP Group 10");
    footer->setObjectName("sidebarFooter");
    footer->setAlignment(Qt::AlignCenter);
    layout->addWidget(footer);
    layout->addSpacing(15);
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
