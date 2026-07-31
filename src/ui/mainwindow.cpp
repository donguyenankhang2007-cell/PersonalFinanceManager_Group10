#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Personal Finance Manager");
    resize(1000, 650);

    setupPages();
    setupConnections();
}

MainWindow::~MainWindow()
{
    delete ui;
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
    btnTransaction = new QPushButton("   Transactions");
    btnCategory    = new QPushButton("   Categories");
    btnBudget      = new QPushButton("   Budget");
    btnReport      = new QPushButton("   Reports");

    sidebarLayout->addWidget(appTitle);
    sidebarLayout->addWidget(btnDashboard);
    sidebarLayout->addWidget(btnTransaction);
    sidebarLayout->addWidget(btnCategory);
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
    );

    dashboard   = new DashboardPage();
    transaction = new TransactionPage();
    category    = new CategoryPage();
    budget      = new BudgetPage();
    report      = new ReportPage();

    stackedWidget->addWidget(dashboard);     // index 0
    stackedWidget->addWidget(transaction);   // index 1
    stackedWidget->addWidget(category);      // index 2
    stackedWidget->addWidget(budget);        // index 3
    stackedWidget->addWidget(report);        // index 4

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
    connect(btnTransaction, &QPushButton::clicked, this, &MainWindow::showTransaction);
    connect(btnCategory, &QPushButton::clicked, this, &MainWindow::showCategory);
    connect(btnBudget, &QPushButton::clicked, this, &MainWindow::showBudget);
    connect(btnReport, &QPushButton::clicked, this, &MainWindow::showReport);
}

void MainWindow::showDashboard()
{
    stackedWidget->setCurrentIndex(0);
}

void MainWindow::showTransaction()
{
    stackedWidget->setCurrentIndex(1);
}

void MainWindow::showCategory()
{
    stackedWidget->setCurrentIndex(2);
}

void MainWindow::showBudget()
{
    stackedWidget->setCurrentIndex(3);
}

void MainWindow::showReport()
{
    stackedWidget->setCurrentIndex(4);
}
