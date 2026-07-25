cmake_minimum_required(VERSION 3.19)

project(PersonalFinanceManager_Group10 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql)

qt_standard_project_setup()

qt_add_executable(PersonalFinanceManager_Group10
    WIN32
    MACOSX_BUNDLE

    # main
    src/main.cpp

    # app
    src/app/AppContext.cpp
    src/app/AppContext.h

    # database
    src/core/database/DatabaseManager.cpp
    src/core/database/DatabaseManager.h

    # models
    src/core/models/Account.cpp
    src/core/models/Account.h

    src/core/models/Budget.cpp
    src/core/models/Budget.h

    src/core/models/Category.cpp
    src/core/models/Category.h

    src/core/models/Report.cpp
    src/core/models/Report.h

    src/core/models/Transaction.cpp
    src/core/models/Transaction.h

    # repositories
    src/core/repositories/AccountRepository.h
    src/core/repositories/CategoryRepository.h
    src/core/repositories/TransactionRepository.h

    # services
    src/core/services/BudgetService.h
    src/core/services/ReportService.h
    src/core/services/TransactionService.h

    # ui
    src/ui/mainwindow.cpp
    src/ui/mainwindow.h
    src/ui/mainwindow.ui

    src/ui/dialogs/CategoryDialog.h
    src/ui/dialogs/TransactionDialog.h

    src/ui/pages/BudgetPage.h
    src/ui/pages/CategoryPage.h
    src/ui/pages/DashboardPage.h
    src/ui/pages/ReportPage.h
    src/ui/pages/TransactionPage.h

    # utils
    src/utils/DateUtils.h
    src/utils/MoneyUtils.h
)

target_link_libraries(PersonalFinanceManager_Group10
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
)