cmake_minimum_required(VERSION 3.19)
project(PersonalFinanceManager_Group10 LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

qt_add_executable(PersonalFinanceManager_Group10
    MANUAL_FINALIZATION
    MACOSX_BUNDLE
    WIN32
    src/main.cpp
    src/ui/mainwindow.cpp
    src/ui/mainwindow.h
    src/ui/mainwindow.ui
)
target_link_libraries(PersonalFinanceManager_Group10
    PRIVATE
        Qt::Core
        Qt::Widgets
)
target_include_directories(PersonalFinanceManager_Group10 PRIVATE
    src
    src/ui
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
