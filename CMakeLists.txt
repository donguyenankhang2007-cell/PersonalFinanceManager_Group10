cmake_minimum_required(VERSION 3.19)
project(PersonalFinanceManager_Group10 LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Widgets
    Sql
)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON) 

qt_add_executable(PersonalFinanceManager_Group10
    WIN32 MACOSX_BUNDLE

    src/main.cpp

    src/ui/mainwindow.cpp
    src/ui/mainwindow.h
    src/ui/mainwindow.ui

    src/core/database/DatabaseManager.h
    src/core/database/DatabaseManager.cpp

    src/core/models/Transaction.h
    src/core/models/Transaction.cpp

    src/core/models/Category.h
    src/core/models/Category.cpp

    src/core/models/Account.h
    src/core/models/Account.cpp

    src/core/models/Budget.h
    src/core/models/Budget.cpp

    src/core/repositories/AccountRepository.h
    src/core/repositories/AccountRepository.cpp

    src/core/repositories/CategoryRepository.h
    src/core/repositories/CategoryRepository.cpp

    src/core/repositories/TransactionRepository.h
    src/core/repositories/TransactionRepository.cpp

    src/core/repositories/BudgetRepository.h
    src/core/repositories/BudgetRepository.cpp

    resources/app.qrc
)

target_link_libraries(PersonalFinanceManager_Group10
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Sql
)

install(TARGETS PersonalFinanceManager_Group10
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET PersonalFinanceManager_Group10
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
