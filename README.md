# 💰 Personal Finance Manager — Group 10

> **Môn học:** Lập trình Hướng đối tượng (OOP)  
> **Ngôn ngữ:** C++ 17 · **Framework:** Qt 6 · **Database:** SQLite  
> **Build System:** CMake ≥ 3.19

---

## 📑 Mục lục

1. [Thành viên & Phân công](#1--thành-viên--phân-công)
2. [Tổng quan kiến trúc](#2--tổng-quan-kiến-trúc)
3. [Cây thư mục dự án](#3--cây-thư-mục-dự-án)
4. [Hướng dẫn cài đặt & Chạy dự án](#4--hướng-dẫn-cài-đặt--chạy-dự-án)
5. [Design Patterns sử dụng (có code mẫu)](#5--design-patterns-sử-dụng-có-code-mẫu)
6. [Coding Convention (Quy tắc đặt tên)](#6--coding-convention-quy-tắc-đặt-tên)
7. [Quy trình Git Workflow](#7--quy-trình-git-workflow)
8. [Phân công công việc chi tiết](#8--phân-công-công-việc-chi-tiết)
9. [Hướng dẫn viết & chạy Unit Test](#9--hướng-dẫn-viết--chạy-unit-test)
10. [Tài liệu tham khảo](#10--tài-liệu-tham-khảo)

---

## 1. 👥 Thành viên & Phân công

| STT | Họ và tên       | Vai trò                                        | Branch Git   |
|:---:|:----------------|:-----------------------------------------------|:-------------|
| 1   | **An Khang**    | System Architect + OOP Core (Kiến trúc, UML, OOP, Design Pattern) | `AnKhang`    |
| 2   | **Hữu Lam**    | Data + Database (Lưu dữ liệu, File/SQLite)    | `HuuLam`     |
| 3   | **Minh Hạo**    | Financial Logic Developer (Expense, Budget, Report, tính toán) | `MinhHao`    |
| 4   | **Việt Tường**  | GUI Developer — Qt (Giao diện người dùng)      | `VietTuong`  |
| 5   | **Gia Hưng**    | Tester + Integration + Presentation (Test, ghép code, demo) | `Hung`       |

---

## 2. 🏗️ Tổng quan kiến trúc

Dự án sử dụng kiến trúc **Layered Architecture** (kiến trúc phân tầng) kết hợp với các Design Pattern OOP. Mỗi tầng chỉ giao tiếp với tầng liền kề, giúp dễ bảo trì và phân công.

```
┌──────────────────────────────────────────────────┐
│                 PRESENTATION LAYER                │
│        (Qt Widgets: Pages + Dialogs)              │
│   DashboardPage │ TransactionPage │ ReportPage    │
├──────────────────────────────────────────────────┤
│                  SERVICE LAYER                    │
│        (Business Logic / Nghiệp vụ)              │
│  TransactionService │ BudgetService │ ReportService│
├──────────────────────────────────────────────────┤
│                REPOSITORY LAYER                   │
│      (Truy vấn dữ liệu / Data Access)            │
│ TransactionRepo │ CategoryRepo │ AccountRepo      │
├──────────────────────────────────────────────────┤
│                  DATA LAYER                       │
│     (SQLite Database + DatabaseManager)           │
│           database_schema.sql                     │
└──────────────────────────────────────────────────┘
```

**Luồng dữ liệu:**
```
[Người dùng nhấn nút] → Page/Dialog → Service → Repository → DatabaseManager → SQLite
```

---

## 3. 📂 Cây thư mục dự án

```
PersonalFinanceManager_Group10/
│
├── main.cpp                      # Entry point — khởi tạo QApplication
├── mainwindow.h / .cpp / .ui     # Cửa sổ chính (chứa menu, navigation)
├── CMakeLists.txt                # Cấu hình build CMake
│
├── src/                          # ★ TOÀN BỘ MÃ NGUỒN CHÍNH
│   ├── app/                      # Quản lý vòng đời ứng dụng
│   │   ├── AppContext.h          # Singleton — giữ tham chiếu tới tất cả Service
│   │   └── AppContext.cpp
│   │
│   ├── core/                     # Tầng logic & dữ liệu
│   │   ├── models/               # ★ Data Models (các lớp dữ liệu)
│   │   │   ├── Transaction.h     # Giao dịch thu/chi
│   │   │   ├── Category.h        # Danh mục (Ăn uống, Di chuyển,...)
│   │   │   ├── Account.h         # Tài khoản (Tiền mặt, Ngân hàng,...)
│   │   │   └── Budget.h          # Ngân sách theo danh mục
│   │   │
│   │   ├── repositories/         # ★ Data Access Layer (CRUD với DB)
│   │   │   ├── TransactionRepository.h
│   │   │   ├── CategoryRepository.h
│   │   │   └── AccountRepository.h
│   │   │
│   │   ├── services/             # ★ Business Logic Layer (xử lý nghiệp vụ)
│   │   │   ├── TransactionService.h
│   │   │   ├── BudgetService.h
│   │   │   └── ReportService.h
│   │   │
│   │   └── database/             # Kết nối & quản lý SQLite
│   │       ├── DatabaseManager.h
│   │       └── DatabaseManager.cpp
│   │
│   ├── ui/                       # Tầng giao diện Qt
│   │   ├── pages/                # Các trang chính
│   │   │   ├── DashboardPage.h   # Trang tổng quan
│   │   │   ├── TransactionPage.h # Trang danh sách giao dịch
│   │   │   ├── CategoryPage.h    # Trang quản lý danh mục
│   │   │   └── ReportPage.h      # Trang báo cáo thống kê
│   │   │
│   │   └── dialogs/              # Hộp thoại popup
│   │       ├── TransactionDialog.h  # Thêm/Sửa giao dịch
│   │       └── CategoryDialog.h     # Thêm/Sửa danh mục
│   │
│   ├── utils/                    # Hàm tiện ích dùng chung
│   │   ├── DateUtils.h           # Xử lý ngày tháng
│   │   └── MoneyUtils.h          # Định dạng tiền tệ
│   │
│   └── main.cpp                  # (Placeholder — chưa sử dụng)
│
├── data/                         # File dữ liệu
│   └── database_schema.sql       # SQL tạo bảng ban đầu
│
├── resources/                    # Tài nguyên (icon, ảnh, ...)
│   ├── app.qrc                   # Qt Resource File
│   └── icons/                    # Thư mục chứa icon
│
├── tests/                        # Unit Tests
│   ├── CMakeLists.txt            # Cấu hình build cho test
│   └── TransactionServiceTest.cpp
│
├── docs/                         # Tài liệu dự án
│   ├── architecture.md           # Mô tả kiến trúc
│   └── coding-convention.md      # Quy tắc code
│
└── build/                        # Thư mục build (TỰ SINH — KHÔNG COMMIT)
```

> ⚠️ **Lưu ý:** Thư mục `build/` là do CMake tự sinh ra. **KHÔNG BAO GIỜ** commit thư mục này lên Git.

---

## 4. 🚀 Hướng dẫn cài đặt & Chạy dự án

### Yêu cầu hệ thống

| Công cụ     | Phiên bản tối thiểu | Ghi chú                                  |
|:-----------|:--------------------:|:------------------------------------------|
| Qt         | 6.5+                 | Bao gồm Qt Widgets, Qt SQL               |
| CMake      | 3.19+                | Thường đi kèm khi cài Qt                 |
| C++ Compiler | C++17              | MinGW (Windows) hoặc Clang (macOS)       |
| SQLite     | 3.x                  | Đã tích hợp sẵn trong Qt SQL module      |

### Bước 1: Cài đặt Qt

1. Tải **Qt Online Installer** tại: https://www.qt.io/download-open-source
2. Chạy installer, chọn cài đặt:
   - ✅ Qt 6.x (phiên bản mới nhất, ví dụ: Qt 6.11.1)
   - ✅ Qt Creator (IDE)
   - ✅ MinGW 64-bit (Windows) hoặc Desktop gcc (Linux) — macOS dùng Clang mặc định
   - ✅ Qt 6 → Additional Libraries → **Qt SQL** (để dùng SQLite)

### Bước 2: Clone dự án

```bash
git clone <URL-repo-của-nhóm>
cd PersonalFinanceManager_Group10
```

### Bước 3: Mở & Chạy dự án

#### Cách A: Dùng Qt Creator (Khuyến nghị)

1. Mở **Qt Creator**
2. Chọn `File` → `Open File or Project...`
3. Trỏ tới file `CMakeLists.txt` trong thư mục gốc dự án
4. Chọn Kit build (ví dụ: `Desktop Qt 6.11.1 MinGW 64-bit`)
5. Nhấn nút **Run** ▶️ (hoặc `Ctrl+R` / `Cmd+R`)

#### Cách B: Dùng Terminal (Command Line)

```bash
# Tạo thư mục build & cấu hình
cmake -B build -DCMAKE_PREFIX_PATH=<đường-dẫn-tới-Qt>

# Biên dịch
cmake --build build

# Chạy ứng dụng
# Windows:
.\build\PersonalFinanceManager_Group10.exe

# macOS:
./build/PersonalFinanceManager_Group10.app/Contents/MacOS/PersonalFinanceManager_Group10

# Linux:
./build/PersonalFinanceManager_Group10
```

---

## 5. 🎨 Design Patterns sử dụng (có code mẫu)

Dự án OOP này sử dụng **5 Design Patterns** chính. Dưới đây là giải thích và code mẫu cụ thể cho từng pattern trong dự án.

---

### 5.1 Singleton Pattern — `AppContext`

**Mục đích:** Đảm bảo chỉ có **duy nhất một instance** của `AppContext` trong toàn bộ ứng dụng. `AppContext` giữ tham chiếu tới tất cả các Service, giúp mọi nơi đều truy cập được cùng một bộ Service.

**Ai viết:** An Khang (System Architect)

**File:** `src/app/AppContext.h` + `src/app/AppContext.cpp`

```cpp
// ============================================
// File: src/app/AppContext.h
// Pattern: Singleton
// ============================================
#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include "../core/services/TransactionService.h"
#include "../core/services/BudgetService.h"
#include "../core/services/ReportService.h"
#include "../core/database/DatabaseManager.h"

class AppContext {
public:
    // Truy cập instance duy nhất (Singleton)
    static AppContext& getInstance() {
        static AppContext instance;   // Khởi tạo 1 lần duy nhất
        return instance;
    }

    // Khởi tạo tất cả thành phần
    void initialize(const QString& dbPath);

    // Getter cho các Service
    TransactionService* getTransactionService() { return m_transactionService; }
    BudgetService*      getBudgetService()      { return m_budgetService; }
    ReportService*      getReportService()       { return m_reportService; }
    DatabaseManager*    getDatabaseManager()     { return m_dbManager; }

private:
    // ★ Constructor private — không cho phép tạo từ bên ngoài
    AppContext() = default;

    // ★ Xóa copy constructor & assignment — không cho phép sao chép
    AppContext(const AppContext&) = delete;
    AppContext& operator=(const AppContext&) = delete;

    // Các thành phần được quản lý
    DatabaseManager*     m_dbManager = nullptr;
    TransactionService*  m_transactionService = nullptr;
    BudgetService*       m_budgetService = nullptr;
    ReportService*       m_reportService = nullptr;
};

#endif // APPCONTEXT_H
```

```cpp
// ============================================
// File: src/app/AppContext.cpp
// ============================================
#include "AppContext.h"

void AppContext::initialize(const QString& dbPath) {
    // Tạo các đối tượng theo đúng thứ tự phụ thuộc
    m_dbManager          = new DatabaseManager(dbPath);
    m_transactionService = new TransactionService(m_dbManager);
    m_budgetService      = new BudgetService(m_dbManager);
    m_reportService      = new ReportService(m_dbManager);
}
```

**Cách sử dụng ở bất kỳ đâu trong dự án:**
```cpp
// Truy cập TransactionService từ bất kỳ file nào
auto* txService = AppContext::getInstance().getTransactionService();
auto transactions = txService->getAllTransactions();
```

---

### 5.2 Repository Pattern — Tách biệt Logic và Database

**Mục đích:** Tạo một tầng trung gian giữa **Business Logic** (Service) và **Database**. Service không cần biết dữ liệu lưu ở đâu (SQLite, file text, hay cloud) — chỉ cần gọi hàm của Repository.

**Ai viết:** Hữu Lam (Data + Database)

**File mẫu:** `src/core/repositories/TransactionRepository.h`

```cpp
// ============================================
// File: src/core/repositories/TransactionRepository.h
// Pattern: Repository
// ============================================
#ifndef TRANSACTIONREPOSITORY_H
#define TRANSACTIONREPOSITORY_H

#include "../models/Transaction.h"
#include "../database/DatabaseManager.h"
#include <vector>
#include <QString>

class TransactionRepository {
public:
    explicit TransactionRepository(DatabaseManager* dbManager)
        : m_db(dbManager) {}

    // ★ CRUD Operations (Create, Read, Update, Delete)

    // Thêm giao dịch mới → trả về ID
    int add(const Transaction& transaction);

    // Lấy tất cả giao dịch
    std::vector<Transaction> getAll();

    // Lấy giao dịch theo ID
    Transaction getById(int id);

    // Lấy giao dịch theo khoảng thời gian
    std::vector<Transaction> getByDateRange(const QString& startDate,
                                            const QString& endDate);

    // Lấy giao dịch theo danh mục
    std::vector<Transaction> getByCategoryId(int categoryId);

    // Cập nhật giao dịch
    bool update(const Transaction& transaction);

    // Xóa giao dịch theo ID
    bool remove(int id);

private:
    DatabaseManager* m_db;  // Con trỏ tới DatabaseManager (không sở hữu)
};

#endif // TRANSACTIONREPOSITORY_H
```

---

### 5.3 MVC Pattern (Model-View-Controller) — Cấu trúc Qt

**Mục đích:** Tách biệt 3 thành phần: **Model** (dữ liệu), **View** (giao diện), **Controller** (xử lý sự kiện). Trong Qt, Controller thường được tích hợp vào View thông qua cơ chế **Signals & Slots**.

**Ai viết:** Việt Tường (GUI Developer) phối hợp với An Khang

```
┌─────────────────┐     Signal/Slot     ┌──────────────────┐
│      VIEW       │ ◄─────────────────► │   CONTROLLER     │
│  (Qt Widgets)   │                     │ (Page class với  │
│  .ui file       │                     │  slots xử lý)    │
└─────────────────┘                     └───────┬──────────┘
                                                │ gọi
                                        ┌───────▼──────────┐
                                        │     MODEL        │
                                        │ (Service Layer)  │
                                        └──────────────────┘
```

**File mẫu:** `src/ui/pages/TransactionPage.h`

```cpp
// ============================================
// File: src/ui/pages/TransactionPage.h
// Pattern: MVC (View + Controller kết hợp trong Qt)
// ============================================
#ifndef TRANSACTIONPAGE_H
#define TRANSACTIONPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include "../../core/services/TransactionService.h"

class TransactionPage : public QWidget {
    Q_OBJECT

public:
    explicit TransactionPage(TransactionService* service,
                             QWidget* parent = nullptr);

private slots:
    // ★ Controller: Xử lý sự kiện từ giao diện
    void onAddButtonClicked();       // Bấm nút "Thêm giao dịch"
    void onEditButtonClicked();      // Bấm nút "Sửa"
    void onDeleteButtonClicked();    // Bấm nút "Xóa"
    void onSearchTextChanged(const QString& text);  // Gõ ô tìm kiếm

private:
    void setupUI();       // Khởi tạo giao diện
    void loadData();      // Tải dữ liệu từ Service lên bảng

    // ★ View: Các widget giao diện
    QTableWidget*  m_table;
    QPushButton*   m_addButton;
    QPushButton*   m_editButton;
    QPushButton*   m_deleteButton;

    // ★ Model: Tham chiếu tới Service Layer
    TransactionService* m_service;
};

#endif // TRANSACTIONPAGE_H
```

---

### 5.4 Inheritance & Polymorphism — Hệ thống Model

**Mục đích:** Thể hiện tính **kế thừa** và **đa hình** — yêu cầu cốt lõi của OOP. Tất cả Model đều kế thừa từ một lớp cơ sở `BaseModel` chứa các thuộc tính chung.

**Ai viết:** An Khang (OOP Core)

```cpp
// ============================================
// File: src/core/models/BaseModel.h (NÊN TẠO THÊM)
// Pattern: Inheritance + Polymorphism
// ============================================
#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QString>

class BaseModel {
public:
    BaseModel() : m_id(0) {}
    explicit BaseModel(int id) : m_id(id) {}
    virtual ~BaseModel() = default;   // ★ Virtual destructor — BẮT BUỘC cho OOP

    // Getter / Setter chung
    int getId() const { return m_id; }
    void setId(int id) { m_id = id; }

    // ★ Pure virtual — buộc lớp con phải override (Đa hình)
    virtual QString toString() const = 0;

    // ★ Virtual — lớp con CÓ THỂ override
    virtual QString getDisplayName() const { return "BaseModel"; }

protected:
    int m_id;
};

#endif // BASEMODEL_H
```

```cpp
// ============================================
// File: src/core/models/Transaction.h
// Kế thừa từ BaseModel
// ============================================
#ifndef TRANSACTION_H
#define TRANSACTION_H

#include "BaseModel.h"
#include <QString>

// Enum phân loại loại giao dịch
enum class TransactionType {
    INCOME,     // Thu nhập
    EXPENSE     // Chi tiêu
};

class Transaction : public BaseModel {
public:
    Transaction() : BaseModel(), m_amount(0.0), m_type(TransactionType::EXPENSE) {}

    Transaction(int id, double amount, const QString& description,
                const QString& date, int categoryId, TransactionType type)
        : BaseModel(id)
        , m_amount(amount)
        , m_description(description)
        , m_date(date)
        , m_categoryId(categoryId)
        , m_type(type) {}

    // ★ Override pure virtual từ BaseModel (Đa hình)
    QString toString() const override {
        return QString("[%1] %2: %3 VND - %4")
            .arg(m_type == TransactionType::INCOME ? "THU" : "CHI")
            .arg(m_date)
            .arg(m_amount, 0, 'f', 0)
            .arg(m_description);
    }

    QString getDisplayName() const override {
        return m_description;
    }

    // Getters
    double          getAmount()      const { return m_amount; }
    QString         getDescription() const { return m_description; }
    QString         getDate()        const { return m_date; }
    int             getCategoryId()  const { return m_categoryId; }
    TransactionType getType()        const { return m_type; }

    // Setters
    void setAmount(double amount)              { m_amount = amount; }
    void setDescription(const QString& desc)   { m_description = desc; }
    void setDate(const QString& date)          { m_date = date; }
    void setCategoryId(int catId)              { m_categoryId = catId; }
    void setType(TransactionType type)         { m_type = type; }

private:
    double          m_amount;
    QString         m_description;
    QString         m_date;
    int             m_categoryId;
    TransactionType m_type;
};

#endif // TRANSACTION_H
```

```cpp
// ============================================
// File: src/core/models/Category.h
// Kế thừa từ BaseModel
// ============================================
#ifndef CATEGORY_H
#define CATEGORY_H

#include "BaseModel.h"

class Category : public BaseModel {
public:
    Category() : BaseModel() {}
    Category(int id, const QString& name, const QString& icon)
        : BaseModel(id), m_name(name), m_icon(icon) {}

    // ★ Override (Đa hình)
    QString toString() const override {
        return QString("Category [%1]: %2").arg(m_id).arg(m_name);
    }

    QString getDisplayName() const override { return m_name; }

    // Getters & Setters
    QString getName() const { return m_name; }
    QString getIcon() const { return m_icon; }
    void setName(const QString& name) { m_name = name; }
    void setIcon(const QString& icon) { m_icon = icon; }

private:
    QString m_name;
    QString m_icon;
};

#endif // CATEGORY_H
```

**Minh họa Đa hình (Polymorphism) trong thực tế:**
```cpp
// Tạo mảng con trỏ BaseModel — chứa nhiều loại đối tượng khác nhau
std::vector<BaseModel*> items;
items.push_back(new Transaction(1, 50000, "Ăn trưa", "2026-07-24", 1, TransactionType::EXPENSE));
items.push_back(new Category(1, "Ăn uống", "🍔"));

// ★ Đa hình: cùng gọi toString() nhưng kết quả khác nhau
for (const auto* item : items) {
    qDebug() << item->toString();
    // Transaction → "[CHI] 2026-07-24: 50000 VND - Ăn trưa"
    // Category   → "Category [1]: Ăn uống"
}
```

---

### 5.5 Observer Pattern (Signals & Slots) — Giao tiếp giữa các thành phần

**Mục đích:** Khi dữ liệu thay đổi ở một nơi (ví dụ: thêm giao dịch), các nơi khác (Dashboard, Report) tự động cập nhật mà không cần kiểm tra thủ công. Qt hỗ trợ sẵn qua cơ chế **Signals & Slots**.

**Ai viết:** Việt Tường (GUI) kết hợp Minh Hạo (Logic)

```cpp
// ============================================
// File: src/core/services/TransactionService.h
// Pattern: Observer (Qt Signals & Slots)
// ============================================
#ifndef TRANSACTIONSERVICE_H
#define TRANSACTIONSERVICE_H

#include <QObject>
#include "../models/Transaction.h"
#include "../repositories/TransactionRepository.h"
#include <vector>

class TransactionService : public QObject {
    Q_OBJECT    // ★ Macro bắt buộc để dùng Signals & Slots

public:
    explicit TransactionService(DatabaseManager* db, QObject* parent = nullptr)
        : QObject(parent), m_repo(db) {}

    // Thêm giao dịch mới
    int addTransaction(const Transaction& t) {
        int id = m_repo.add(t);
        if (id > 0) {
            emit transactionAdded(t);    // ★ Phát signal thông báo
        }
        return id;
    }

    // Xóa giao dịch
    bool removeTransaction(int id) {
        bool ok = m_repo.remove(id);
        if (ok) {
            emit transactionRemoved(id);  // ★ Phát signal thông báo
        }
        return ok;
    }

    // Lấy tất cả giao dịch
    std::vector<Transaction> getAllTransactions() {
        return m_repo.getAll();
    }

    // Tính tổng thu/chi
    double getTotalIncome();
    double getTotalExpense();
    double getBalance();

signals:
    // ★ Signals — phát ra khi dữ liệu thay đổi
    void transactionAdded(const Transaction& transaction);
    void transactionUpdated(const Transaction& transaction);
    void transactionRemoved(int transactionId);

private:
    TransactionRepository m_repo;
};

#endif // TRANSACTIONSERVICE_H
```

**Cách sử dụng Observer (kết nối Signal → Slot):**
```cpp
// Trong DashboardPage — tự động cập nhật khi có giao dịch mới
auto* txService = AppContext::getInstance().getTransactionService();

// ★ Kết nối: Khi transactionAdded() được phát, gọi refreshDashboard()
connect(txService, &TransactionService::transactionAdded,
        this,      &DashboardPage::refreshDashboard);

// Bây giờ, mỗi khi bất kỳ nơi nào gọi addTransaction(),
// DashboardPage sẽ TỰ ĐỘNG gọi refreshDashboard() để cập nhật giao diện!
```

---

### 5.6 Tóm tắt Design Patterns

| # | Pattern                | Thể hiện OOP            | File chính                  | Người phụ trách |
|:-:|:-----------------------|:------------------------|:----------------------------|:----------------|
| 1 | **Singleton**          | Encapsulation           | `AppContext.h`              | An Khang        |
| 2 | **Repository**         | Abstraction             | `*Repository.h`            | Hữu Lam         |
| 3 | **MVC**                | Separation of Concerns  | `*Page.h` + `*Service.h`   | Việt Tường + Minh Hạo |
| 4 | **Inheritance/Polymorphism** | Inheritance, Polymorphism | `BaseModel.h` + `Transaction.h` | An Khang |
| 5 | **Observer (Signal/Slot)** | Loose Coupling       | `TransactionService.h`     | Việt Tường + Minh Hạo |

---

## 6. 📝 Coding Convention (Quy tắc đặt tên)

### 6.1 Đặt tên biến & hàm

| Loại                | Quy tắc                | Ví dụ                         |
|:--------------------|:------------------------|:-------------------------------|
| Tên class           | `PascalCase`            | `TransactionService`, `DashboardPage` |
| Tên hàm / method    | `camelCase`             | `getAll()`, `addTransaction()` |
| Biến member (private)| `m_camelCase`          | `m_amount`, `m_dbManager`      |
| Biến local          | `camelCase`             | `totalIncome`, `startDate`     |
| Hằng số / Enum      | `UPPER_SNAKE_CASE`      | `MAX_AMOUNT`, `TransactionType::INCOME` |
| Tên file header     | `PascalCase.h`          | `Transaction.h`, `BudgetService.h` |
| Tên file source     | `PascalCase.cpp`        | `DatabaseManager.cpp`          |

### 6.2 Cấu trúc file Header (.h)

```cpp
#ifndef CLASSNAME_H          // Include guard
#define CLASSNAME_H

#include <...>               // Thư viện chuẩn / Qt trước
#include "../local/file.h"   // File nội bộ dự án sau

class ClassName {
public:
    // Constructor / Destructor
    ClassName();
    ~ClassName();

    // Public methods (giao diện công khai)
    void doSomething();

signals:                      // (Chỉ có nếu dùng Q_OBJECT)
    void somethingHappened();

private slots:                // (Chỉ có nếu dùng Q_OBJECT)
    void onButtonClicked();

private:
    // Private members
    int m_value;
};

#endif // CLASSNAME_H
```

### 6.3 Quy tắc chung

- ✅ Mỗi class nằm trong **một file .h riêng** (hoặc .h + .cpp nếu cần implementation)
- ✅ Comment bằng **tiếng Việt hoặc tiếng Anh** đều được, nhưng **nhất quán** trong từng file
- ✅ Dùng `const` khi biến không bị thay đổi
- ✅ Dùng `override` khi ghi đè hàm virtual
- ❌ **KHÔNG** dùng `using namespace std;` trong file header
- ❌ **KHÔNG** commit file trong thư mục `build/`

---

## 7. 🔀 Quy trình Git Workflow

### 7.1 Cấu trúc Branch

```
main  ◄──────── Branch chính (sản phẩm hoàn chỉnh, chỉ merge vào đây)
 │
 ├── AnKhang     ◄── An Khang phát triển: Models, AppContext, kiến trúc
 ├── HuuLam      ◄── Hữu Lam phát triển: Database, Repository
 ├── MinhHao     ◄── Minh Hạo phát triển: Service logic
 ├── VietTuong   ◄── Việt Tường phát triển: UI Pages, Dialogs
 └── Hung        ◄── Gia Hưng: Test, ghép code, tạo tài liệu
```

### 7.2 Quy trình làm việc hàng ngày

```bash
# ① Trước khi code — luôn pull code mới nhất
git checkout <tên-branch-của-bạn>
git pull origin <tên-branch-của-bạn>

# ② Code xong — commit với message rõ ràng
git add .
git commit -m "[Tên module] Mô tả ngắn gọn"

# ③ Push lên remote
git push origin <tên-branch-của-bạn>
```

### 7.3 Quy tắc viết Commit Message

```
[Module] Hành động + Mô tả

Ví dụ:
[Model] Thêm class Transaction kế thừa BaseModel
[DB] Hoàn thành DatabaseManager với CRUD operations
[UI] Tạo TransactionPage với QTableWidget
[Service] Thêm hàm tính tổng thu/chi trong TransactionService
[Test] Viết unit test cho TransactionService::addTransaction
[Fix] Sửa lỗi crash khi xóa giao dịch không tồn tại
[Docs] Cập nhật README với hướng dẫn chạy dự án
```

### 7.4 Quy trình ghép code (Merge) — Dành cho Gia Hưng

```bash
# ① Chuyển sang branch Hung
git checkout Hung

# ② Kéo code mới nhất của thành viên cần ghép
git pull origin AnKhang      # Pull code của An Khang
git pull origin HuuLam       # Pull code của Hữu Lam

# ③ Merge từng branch vào Hung
git merge AnKhang
# → Nếu có CONFLICT: mở file bị conflict, sửa thủ công, rồi:
git add <file-bị-conflict>
git commit -m "[Merge] Ghép code AnKhang vào Hung"

# ④ Build & Test trên branch Hung
cmake --build build
./build/PersonalFinanceManager_Group10

# ⑤ Khi mọi thứ ổn → Merge vào main
git checkout main
git merge Hung
git push origin main
```

### 7.5 Xử lý Conflict (Xung đột code)

Khi merge mà gặp conflict, Git sẽ đánh dấu trong file như sau:
```cpp
<<<<<<< HEAD
// Code hiện tại của bạn
int value = 100;
=======
// Code từ branch khác
int value = 200;
>>>>>>> AnKhang
```

**Cách xử lý:**
1. Mở file bị conflict
2. Xóa các dấu `<<<<<<<`, `=======`, `>>>>>>>`
3. Giữ lại phần code đúng (hoặc kết hợp cả hai nếu cần)
4. `git add <file>` → `git commit`

---

## 8. 📋 Phân công công việc chi tiết

### Giai đoạn 1: Nền tảng (Foundation) — Ưu tiên cao nhất

| Người        | Công việc                                         | File                                    | Trạng thái |
|:-------------|:--------------------------------------------------|:----------------------------------------|:----------:|
| **An Khang** | Tạo `BaseModel.h` với virtual destructor & toString | `src/core/models/BaseModel.h`          | ⬜ TODO     |
| **An Khang** | Hoàn thành `Transaction.h` kế thừa BaseModel      | `src/core/models/Transaction.h`         | ✅ DONE     |
| **An Khang** | Hoàn thành `Category.h` kế thừa BaseModel         | `src/core/models/Category.h`            | ✅ DONE     |
| **An Khang** | Hoàn thành `Account.h` kế thừa BaseModel          | `src/core/models/Account.h`             | ✅ DONE     |
| **An Khang** | Hoàn thành `Budget.h` kế thừa BaseModel           | `src/core/models/Budget.h`              | ✅ DONE     |
| **An Khang** | Viết `AppContext.h/.cpp` (Singleton Pattern)       | `src/app/AppContext.h/.cpp`             | ✅ DONE     |
| **Hữu Lam** | Viết `database_schema.sql` (CREATE TABLE)          | `data/database_schema.sql`              | ✅ DONE     |
| **Hữu Lam** | Viết `DatabaseManager.h/.cpp` (kết nối SQLite)     | `src/core/database/DatabaseManager.h/.cpp` | ✅ DONE  |

### Giai đoạn 2: Tầng dữ liệu & Logic — Cần GĐ1 xong trước

| Người         | Công việc                                          | File                                     | Trạng thái |
|:--------------|:---------------------------------------------------|:-----------------------------------------|:----------:|
| **Hữu Lam**  | Viết `TransactionRepository.h` (CRUD giao dịch)    | `src/core/repositories/TransactionRepository.h` | ✅ DONE |
| **Hữu Lam**  | Viết `CategoryRepository.h` (CRUD danh mục)        | `src/core/repositories/CategoryRepository.h`    | ✅ DONE |
| **Hữu Lam**  | Viết `AccountRepository.h` (CRUD tài khoản)        | `src/core/repositories/AccountRepository.h`     | ✅ DONE |
| **Minh Hạo**  | Viết `TransactionService.h` (thêm/xóa/sửa + signal) | `src/core/services/TransactionService.h`     | ✅ DONE |
| **Minh Hạo**  | Viết `BudgetService.h` (kiểm tra ngân sách)        | `src/core/services/BudgetService.h`           | ✅ DONE |
| **Minh Hạo**  | Viết `ReportService.h` (thống kê thu/chi)           | `src/core/services/ReportService.h`           | ✅ DONE |
| **Minh Hạo**  | Viết `DateUtils.h` + `MoneyUtils.h`                 | `src/utils/DateUtils.h`, `MoneyUtils.h`       | ✅ DONE |

### Giai đoạn 3: Giao diện (UI) — Cần GĐ2 xong trước

| Người           | Công việc                                         | File                                     | Trạng thái |
|:----------------|:--------------------------------------------------|:-----------------------------------------|:----------:|
| **Việt Tường**  | Thiết kế `DashboardPage` (tổng quan tài chính)     | `src/ui/pages/DashboardPage.h`           | ✅ DONE     |
| **Việt Tường**  | Thiết kế `TransactionPage` (danh sách giao dịch)   | `src/ui/pages/TransactionPage.h`         | ✅ DONE     |
| **Việt Tường**  | Thiết kế `CategoryPage` (quản lý danh mục)         | `src/ui/pages/CategoryPage.h`            | ✅ DONE     |
| **Việt Tường**  | Thiết kế `ReportPage` (biểu đồ thống kê)           | `src/ui/pages/ReportPage.h`              | ✅ DONE     |
| **Việt Tường**  | Thiết kế `TransactionDialog` (popup thêm/sửa)      | `src/ui/dialogs/TransactionDialog.h`     | ✅ DONE     |
| **Việt Tường**  | Thiết kế `CategoryDialog` (popup thêm/sửa danh mục)| `src/ui/dialogs/CategoryDialog.h`        | ✅ DONE     |

### Giai đoạn 4: Ghép code & Kiểm thử — Song song với GĐ3

| Người          | Công việc                                          | File / Hành động                          | Trạng thái |
|:---------------|:---------------------------------------------------|:------------------------------------------|:----------:|
| **Gia Hưng**   | Ghép code GĐ1 + GĐ2 + GĐ3 vào branch Hung        | Merge branches                            | ✅ DONE     |
| **Gia Hưng**   | Cập nhật `CMakeLists.txt` thêm tất cả file .cpp     | `CMakeLists.txt`                          | ⬜ TODO     |
| **Gia Hưng**   | Viết unit test cho `TransactionService`              | `tests/TransactionServiceTest.cpp`        | ⬜ TODO     |
| **Gia Hưng**   | Viết unit test cho `BudgetService`                   | `tests/BudgetServiceTest.cpp`             | ⬜ TODO     |
| **Gia Hưng**   | Build & chạy thử toàn bộ ứng dụng                   | Terminal / Qt Creator                     | ⬜ TODO     |
| **Gia Hưng**   | Chuẩn bị slide & demo sản phẩm                      | Presentation                              | ⬜ TODO     |

---

## 9. 🧪 Hướng dẫn viết & chạy Unit Test

### 9.1 Cấu hình CMake cho Tests

File `tests/CMakeLists.txt` cần có nội dung sau:

```cmake
# tests/CMakeLists.txt
find_package(Qt6 REQUIRED COMPONENTS Test)

qt_add_executable(TransactionServiceTest
    TransactionServiceTest.cpp
    ../src/core/services/TransactionService.h
    ../src/core/repositories/TransactionRepository.h
    ../src/core/database/DatabaseManager.h
    ../src/core/database/DatabaseManager.cpp
    ../src/core/models/Transaction.h
    ../src/core/models/BaseModel.h
)

target_link_libraries(TransactionServiceTest PRIVATE
    Qt::Core
    Qt::Test
    Qt::Sql
)
```

Thêm dòng sau vào `CMakeLists.txt` ở thư mục gốc:

```cmake
# Thêm ở cuối CMakeLists.txt gốc
add_subdirectory(tests)
```

### 9.2 Viết Unit Test với Qt Test

```cpp
// ============================================
// File: tests/TransactionServiceTest.cpp
// Hướng dẫn viết Unit Test với Qt Test
// ============================================
#include <QtTest/QtTest>
#include "../src/core/services/TransactionService.h"
#include "../src/core/database/DatabaseManager.h"

class TransactionServiceTest : public QObject {
    Q_OBJECT

private:
    TransactionService* service;
    DatabaseManager*    dbManager;

private slots:
    // ★ Chạy TRƯỚC tất cả test — chuẩn bị môi trường
    void initTestCase() {
        dbManager = new DatabaseManager(":memory:");  // DB trong RAM, không tạo file
        service = new TransactionService(dbManager);
        qDebug() << "=== Bắt đầu chạy test TransactionService ===";
    }

    // ★ Chạy SAU tất cả test — dọn dẹp
    void cleanupTestCase() {
        delete service;
        delete dbManager;
        qDebug() << "=== Kết thúc test ===";
    }

    // --- CÁC HÀM TEST ---

    void testAddTransaction() {
        Transaction t(0, 50000, "Ăn trưa", "2026-07-24", 1, TransactionType::EXPENSE);

        int id = service->addTransaction(t);

        QVERIFY(id > 0);                          // Kiểm tra: ID phải > 0
        qDebug() << "✅ testAddTransaction PASSED";
    }

    void testGetAllTransactions() {
        auto list = service->getAllTransactions();

        QVERIFY(list.size() >= 1);                 // Kiểm tra: danh sách không rỗng
        QCOMPARE(list[0].getDescription(), QString("Ăn trưa"));
        qDebug() << "✅ testGetAllTransactions PASSED";
    }

    void testTotalExpense() {
        double total = service->getTotalExpense();

        QCOMPARE(total, 50000.0);                  // Kiểm tra: tổng chi = 50000
        qDebug() << "✅ testTotalExpense PASSED";
    }

    void testRemoveTransaction() {
        bool ok = service->removeTransaction(1);

        QVERIFY(ok == true);                       // Kiểm tra: xóa thành công
        auto list = service->getAllTransactions();
        QVERIFY(list.empty());                     // Kiểm tra: danh sách rỗng
        qDebug() << "✅ testRemoveTransaction PASSED";
    }
};

QTEST_MAIN(TransactionServiceTest)
#include "TransactionServiceTest.moc"
```

### 9.3 Chạy Unit Test

```bash
# Build tất cả (bao gồm tests)
cmake -B build
cmake --build build

# Chạy test
./build/tests/TransactionServiceTest
```

**Kết quả mong đợi:**
```
********* Start testing of TransactionServiceTest *********
PASS   : TransactionServiceTest::testAddTransaction()
PASS   : TransactionServiceTest::testGetAllTransactions()
PASS   : TransactionServiceTest::testTotalExpense()
PASS   : TransactionServiceTest::testRemoveTransaction()
Totals: 4 passed, 0 failed, 0 skipped
********* Finished testing of TransactionServiceTest *********
```

---

## 10. 📚 Tài liệu tham khảo

| Tài liệu                              | Link                                              |
|:---------------------------------------|:--------------------------------------------------|
| Qt 6 Documentation                     | https://doc.qt.io/qt-6/                           |
| Qt Widgets Tutorial                    | https://doc.qt.io/qt-6/qtwidgets-tutorials.html   |
| Qt Test Framework                      | https://doc.qt.io/qt-6/qttest-index.html          |
| CMake Tutorial                         | https://cmake.org/cmake/help/latest/guide/tutorial/ |
| Design Patterns (Refactoring Guru)     | https://refactoring.guru/design-patterns           |
| Git Workflow Guide                     | https://www.atlassian.com/git/tutorials/comparing-workflows |
| SQLite Documentation                   | https://www.sqlite.org/docs.html                  |

---

> 📅 **Cập nhật lần cuối:** 24/07/2026  
> ✍️ **Tác giả:** Gia Hưng (Tester + Integration)
