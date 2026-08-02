find_package(Qt6 REQUIRED COMPONENTS Test)

# ============================================
# TransactionServiceTest
# ============================================
qt_add_executable(TransactionServiceTest
    TransactionServiceTest.cpp

    # services
    ../src/core/services/TransactionService.cpp
    ../src/core/services/TransactionService.h

    # repositories
    ../src/core/repositories/TransactionRepository.cpp
    ../src/core/repositories/TransactionRepository.h
    ../src/core/repositories/AccountRepository.cpp
    ../src/core/repositories/AccountRepository.h
    ../src/core/repositories/CategoryRepository.cpp
    ../src/core/repositories/CategoryRepository.h

    # database
    ../src/core/database/DatabaseManager.cpp
    ../src/core/database/DatabaseManager.h

    # models
    ../src/core/models/BaseModel.h
    ../src/core/models/Transaction.cpp
    ../src/core/models/Transaction.h
    ../src/core/models/Account.cpp
    ../src/core/models/Account.h
    ../src/core/models/Category.cpp
    ../src/core/models/Category.h
)

target_link_libraries(TransactionServiceTest PRIVATE
    Qt::Core
    Qt::Test
    Qt::Sql
)

# ============================================
# BudgetServiceTest
# ============================================
qt_add_executable(BudgetServiceTest
    BudgetServiceTest.cpp

    # services
    ../src/core/services/BudgetService.cpp
    ../src/core/services/BudgetService.h
    ../src/core/services/TransactionService.cpp
    ../src/core/services/TransactionService.h

    # repositories
    ../src/core/repositories/BudgetRepository.cpp
    ../src/core/repositories/BudgetRepository.h
    ../src/core/repositories/TransactionRepository.cpp
    ../src/core/repositories/TransactionRepository.h
    ../src/core/repositories/AccountRepository.cpp
    ../src/core/repositories/AccountRepository.h
    ../src/core/repositories/CategoryRepository.cpp
    ../src/core/repositories/CategoryRepository.h

    # database
    ../src/core/database/DatabaseManager.cpp
    ../src/core/database/DatabaseManager.h

    # models
    ../src/core/models/BaseModel.h
    ../src/core/models/Budget.cpp
    ../src/core/models/Budget.h
    ../src/core/models/Transaction.cpp
    ../src/core/models/Transaction.h
    ../src/core/models/Account.cpp
    ../src/core/models/Account.h
    ../src/core/models/Category.cpp
    ../src/core/models/Category.h
)

target_link_libraries(BudgetServiceTest PRIVATE
    Qt::Core
    Qt::Test
    Qt::Sql
)
