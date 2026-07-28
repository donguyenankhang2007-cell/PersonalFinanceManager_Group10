find_package(Qt6 REQUIRED COMPONENTS Test)

qt_add_executable(TransactionServiceTest
    TransactionServiceTest.cpp
    ../src/core/services/TransactionService.h
    ../src/core/repositories/TransactionRepository.h
    ../src/core/database/DatabaseManager.h
    ../src/core/database/DatabaseManager.cpp
    ../src/core/models/Transaction.h
)

target_link_libraries(TransactionServiceTest PRIVATE
    Qt::Core
    Qt::Test
    Qt::Sql
)

qt_add_executable(BudgetServiceTest
    BudgetServiceTest.cpp
    ../src/core/services/BudgetService.h
    ../src/core/database/DatabaseManager.h
    ../src/core/database/DatabaseManager.cpp
    ../src/core/models/Budget.h
)

target_link_libraries(BudgetServiceTest PRIVATE
    Qt::Core
    Qt::Test
    Qt::Sql
)
