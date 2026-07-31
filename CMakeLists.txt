cmake_minimum_required(VERSION 3.19)

project(PersonalFinanceManager_Group10 LANGUAGES CXX)

include(GNUInstallDirs)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Sql
)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

qt_add_executable(PersonalFinanceManager_Group10
    MANUAL_FINALIZATION
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
    src/core/models/BaseModel.h
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
    src/core/repositories/AccountRepository.cpp
    src/core/repositories/AccountRepository.h
    src/core/repositories/BudgetRepository.cpp
    src/core/repositories/BudgetRepository.h
    src/core/repositories/CategoryRepository.cpp
    src/core/repositories/CategoryRepository.h
    src/core/repositories/TransactionRepository.cpp
    src/core/repositories/TransactionRepository.h

    # services
    src/core/services/BudgetService.cpp
    src/core/services/BudgetService.h
    src/core/services/ReportService.cpp
    src/core/services/ReportService.h
    src/core/services/TransactionService.cpp
    src/core/services/TransactionService.h

    # ui
    src/ui/mainwindow.cpp
    src/ui/mainwindow.h
    src/ui/mainwindow.ui
    src/ui/dialogs/CategoryDialog.h
    src/ui/dialogs/CategoryDialog.cpp
    src/ui/dialogs/TransactionDialog.h
    src/ui/dialogs/TransactionDialog.cpp
    src/ui/pages/BudgetPage.cpp
    src/ui/pages/BudgetPage.h
    src/ui/pages/CategoryPage.cpp
    src/ui/pages/CategoryPage.h
    src/ui/pages/DashboardPage.cpp
    src/ui/pages/DashboardPage.h
    src/ui/pages/ReportPage.cpp
    src/ui/pages/ReportPage.h
    src/ui/pages/TransactionPage.cpp
    src/ui/pages/TransactionPage.h

    # utils and resources
    src/utils/DateUtils.h
    src/utils/MoneyUtils.h
    resources/app.qrc
)

target_link_libraries(PersonalFinanceManager_Group10
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
)

target_include_directories(PersonalFinanceManager_Group10
    PRIVATE
        src
        src/ui
)

install(TARGETS PersonalFinanceManager_Group10
    BUNDLE DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET PersonalFinanceManager_Group10
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})

qt_finalize_executable(PersonalFinanceManager_Group10)

# # Thêm cấu hình build cho thư mục tests
# add_subdirectory(tests)