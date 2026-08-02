# 💰 Personal Finance Manager — Group 10

> **Môn học:** Lập trình Hướng đối tượng (OOP)
> **Ngôn ngữ:** C++ 17 · **Framework:** Qt 6 (Widgets) · **Database:** SQLite
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
│   Dashboard │ Transaction │ Budget │ Recurring    │
│   Account │ Category │ Report │ Settings          │
├──────────────────────────────────────────────────┤
│                  AppContext (Singleton)           │
├──────────────────────────────────────────────────┤
│                  SERVICE LAYER                    │
│        (Business Logic / Nghiệp vụ + Signals)    │
│  TransactionService │ BudgetService │ ReportService│
│  RecurringTransactionService                       │
├──────────────────────────────────────────────────┤
│                REPOSITORY LAYER                   │
│      (Truy vấn dữ liệu / Data Access)            │
│ TransactionRepo │ CategoryRepo │ AccountRepo      │
│ BudgetRepo │ RecurringTransactionRepo              │
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

**Luồng cập nhật UI (Observer):**
```
Service (thêm/sửa/xóa) → emit signal → các Page đăng ký lắng nghe → tự gọi loadData()
```

---

## 3. 📂 Cây thư mục dự án

```
PersonalFinanceManager_Group10/
│
├── CMakeLists.txt                # Cấu hình build CMake
├── data/                         # File dữ liệu
│   └── database_schema.sql       # SQL tạo bảng ban đầu
│
├── src/                          # ★ TOÀN BỘ MÃ NGUỒN CHÍNH
│   ├── main.cpp                  # Entry point — khởi tạo QApplication + MainWindow
│   ├── app/
│   │   └── AppContext.h/.cpp     # Singleton — cung cấp toàn bộ Service/Repository
│   │
│   ├── core/
│   │   ├── models/               # ★ Data Models (kế thừa BaseModel)
│   │   │   ├── BaseModel.h       # Lớp cơ sở: m_id, getId/setId, toString()...
│   │   │   ├── Account.h/.cpp    # Tài khoản (Tiền mặt, Ngân hàng,...)
│   │   │   ├── Category.h/.cpp   # Danh mục thu/chi
│   │   │   ├── Transaction.h/.cpp# Giao dịch thu/chi
│   │   │   ├── Budget.h/.cpp     # Ngân sách theo danh mục
│   │   │   ├── RecurringTransaction.h/.cpp # Giao dịch định kỳ (daily/weekly/monthly/yearly)
│   │   │   └── Report.h/.cpp     # Kết quả báo cáo
│   │   ├── repositories/         # ★ Data Access Layer (CRUD với DB)
│   │   │   ├── AccountRepository.h/.cpp
│   │   │   ├── CategoryRepository.h/.cpp
│   │   │   ├── TransactionRepository.h/.cpp
│   │   │   ├── BudgetRepository.h/.cpp
│   │   │   └── RecurringTransactionRepository.h/.cpp
│   │   ├── services/             # ★ Business Logic Layer (xử lý nghiệp vụ)
│   │   │   ├── TransactionService.h/.cpp
│   │   │   ├── BudgetService.h/.cpp
│   │   │   ├── RecurringTransactionService.h/.cpp # Sinh giao dịch đến hạn + catch-up
│   │   │   └── ReportService.h/.cpp
│   │   └── database/             # Kết nối & quản lý SQLite
│   │       └── DatabaseManager.h/.cpp
│   │
│   ├── ui/
│   │   ├── mainwindow.h/.cpp     # Cửa sổ chính (sidebar icon thu gọn + QStackedWidget)
│   │   ├── RowActions.h/.cpp     # Icon Edit/Delete cho từng hàng của bảng
│   │   ├── theme/
│   │   │   └── ThemeManager.h/.cpp # Giao diện: 6 accent, Light/Dark mode, density, QSS, QSettings
│   │   ├── pages/                # Các trang chính
│   │   │   ├── DashboardPage.h/.cpp   # Tổng quan: 6 thẻ + budget alerts + top 5 + gần đây + Qt Charts
│   │   │   ├── AccountPage.h/.cpp     # Quản lý tài khoản
│   │   │   ├── CategoryPage.h/.cpp    # Quản lý danh mục
│   │   │   ├── TransactionPage.h/.cpp # Giao dịch + thanh lọc (loại/tài khoản/danh mục/tìm kiếm/khoảng thời gian)
│   │   │   ├── BudgetPage.h/.cpp      # Quản lý ngân sách
│   │   │   ├── ReportPage.h/.cpp      # Báo cáo thống kê + donut + Export CSV
│   │   │   ├── RecurringPage.h/.cpp   # Quản lý giao dịch định kỳ + "Generate due now"
│   │   │   └── SettingsPage.h/.cpp    # Cài đặt: accent, Light/Dark, density, charts, backup/restore DB
│   │   └── dialogs/              # Hộp thoại popup (thêm/sửa)
│   │       ├── AccountDialog.h/.cpp
│   │       ├── CategoryDialog.h/.cpp
│   │       ├── TransactionDialog.h/.cpp
│   │       ├── BudgetDialog.h/.cpp
│   │       └── RecurringTransactionDialog.h/.cpp
│   │
│   └── utils/
│       ├── DateUtils.h           # Xử lý ngày tháng
│       └── MoneyUtils.h          # Định dạng tiền tệ VND
│
├── resources/
│   ├── app.qrc                   # Qt Resource File (chứa database_schema.sql)
│   └── icons/*.svg               # Icon SVG (nav, avatar, edit/delete)
│
├── tests/                        # Unit Tests
│   ├── CMakeLists.txt
│   ├── TransactionServiceTest.cpp
│   └── BudgetServiceTest.cpp
│
├── docs/                         # Tài liệu dự án
│   ├── architecture.md
│   └── coding-convention.md
│
└── build/                        # Thư mục build (TỰ SINH — KHÔNG COMMIT)
```

> ⚠️ **Lưu ý:** Thư mục `build/` là do CMake tự sinh ra. **KHÔNG BAO GIỜ** commit thư mục này lên Git. Giao diện được dựng hoàn toàn bằng code (không còn file `.ui`).

---

## 4. 🚀 Hướng dẫn cài đặt & Chạy dự án

### Yêu cầu hệ thống

| Công cụ     | Phiên bản tối thiểu | Ghi chú                                  |
|:-----------|:--------------------:|:------------------------------------------|
| Qt         | 6.5+                 | Bao gồm Qt Widgets, Qt SQL, Qt Charts   |
| CMake      | 3.19+                | Thường đi kèm khi cài Qt                 |
| C++ Compiler | C++17              | MinGW (Windows) hoặc Clang (macOS)       |
| SQLite     | 3.x                  | Đã tích hợp sẵn trong Qt SQL module      |

### Bước 1: Cài đặt Qt

1. Tải **Qt Online Installer** tại: https://www.qt.io/download-open-source
2. Chạy installer, chọn cài đặt:
   - ✅ Qt 6.x (ví dụ: Qt 6.11.1)
   - ✅ Qt Creator (IDE)
   - ✅ MinGW 64-bit (Windows) hoặc Desktop gcc (Linux) — macOS dùng Clang mặc định
   - ✅ Qt 6 → Additional Libraries → **Qt SQL** (để dùng SQLite)
   - ✅ Qt 6 → Additional Libraries → **Qt Charts** (để hiển thị biểu đồ Dashboard)

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
5. Nhấn nút **Run** ▶️

#### Cách B: Dùng Terminal (Command Line)

```bash
# Cấu hình (thay <Qt-dir> bằng đường dẫn cài Qt)
cmake -B build -DCMAKE_PREFIX_PATH=<Qt-dir>

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

Database được tạo tự động tại `data/finance.db` khi app chạy lần đầu.

### ✨ Tính năng chính

| Tính năng | Mô tả |
|:----------|:------|
| 📊 **Dashboard** | 6 thẻ thống kê (tài khoản, danh mục, giao dịch, tổng thu/chi/số dư), thu/chi/balance tháng này, cảnh báo ngân sách (≥80% amber, ≥100% đỏ), Top 5 danh mục chi, 10 giao dịch gần đây, biểu đồ donut + cột (Qt Charts) |
| 💳 **Tài khoản / Danh mục** | Thêm/sửa/xóa, double-click để sửa nhanh, hiển thị số dư |
| 🧾 **Giao dịch** | Thêm/sửa/xóa + thanh lọc: loại (thu/chi), tài khoản, danh mục, tìm kiếm ghi chú, khoảng thời gian |
| 🎯 **Ngân sách** | Theo dõi chi tiêu theo danh mục, thanh tiến trình so với ngân sách |
| 📈 **Báo cáo** | Thu/chi/balance theo tháng, donut chi theo danh mục, **Export CSV** |
| 🔁 **Giao dịch định kỳ** | Daily/Weekly/Monthly/Yearly, ngày kết thúc tùy chọn; nút **Generate due now** + tự sinh các giao dịch đến hạn khi khởi động |
| 🌙 **Light/Dark mode** | Chuyển theme nhanh trong Settings, lưu lại qua QSettings |
| 🎨 **Giao diện** | 6 preset màu accent, mật độ bảng (density), bật/tắt charts |
| 💾 **Backup/Restore DB** | Sao lưu và khôi phục `finance.db` ngay trong Settings |
| 🖱️ **Sidebar icon** | Tooltip tùy biến, avatar, điều hướng trực quan |

---

## 5. 🎨 Design Patterns sử dụng (có code mẫu)

Dự án OOP này sử dụng **5 Design Patterns** chính. Code mẫu dưới đây đã khớp 100% với code thực tế trong dự án.

### 5.1 Singleton Pattern — `AppContext`

**Mục đích:** Đảm bảo chỉ có **duy nhất một instance** của `AppContext` trong toàn bộ ứng dụng. `AppContext` giữ toàn bộ Service/Repository, giúp mọi nơi truy cập được cùng một bộ dữ liệu.

**File:** `src/app/AppContext.h` + `src/app/AppContext.cpp`

```cpp
// ============================================
// File: src/app/AppContext.h
// Pattern: Singleton
// ============================================
#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include "../core/database/DatabaseManager.h"
#include "../core/repositories/AccountRepository.h"
#include "../core/repositories/TransactionRepository.h"
#include "../core/services/TransactionService.h"
#include "../core/services/BudgetService.h"
#include "../core/services/ReportService.h"
#include "../core/services/RecurringTransactionService.h"

class AppContext
{
public:
    // ★ Truy cập instance duy nhất (Singleton)
    static AppContext& instance();

    DatabaseManager& database();

    AccountRepository& accountRepository();
    TransactionRepository& transactionRepository();
    // ... (CategoryRepository, BudgetRepository, RecurringTransactionRepository)

    TransactionService& transactionService();
    BudgetService& budgetService();
    ReportService& reportService();
    RecurringTransactionService& recurringTransactionService();

private:
    // ★ Constructor private — không cho phép tạo từ bên ngoài
    AppContext();
    ~AppContext();

    // ★ Xóa copy constructor & assignment — không cho phép sao chép
    AppContext(const AppContext&) = delete;
    AppContext& operator=(const AppContext&) = delete;

private:
    AccountRepository m_accountRepository;
    TransactionService m_transactionService;
    // ... (các repository + service còn lại)
};

#endif // APPCONTEXT_H
```

**Cách sử dụng ở bất kỳ đâu trong dự án:**
```cpp
// Truy cập TransactionService từ bất kỳ file nào
auto& txService = AppContext::instance().transactionService();
auto transactions = txService.getAllTransactions();
```

### 5.2 Repository Pattern — Tách biệt Logic và Database

**Mục đích:** Tạo một tầng trung gian giữa **Business Logic** (Service) và **Database**. Service không cần biết dữ liệu lưu ở đâu — chỉ cần gọi hàm của Repository.

**File:** `src/core/repositories/TransactionRepository.h`

```cpp
// ============================================
// File: src/core/repositories/TransactionRepository.h
// Pattern: Repository
// ============================================
class TransactionRepository
{
public:
    // ★ CRUD Operations (Create, Read, Update, Delete)
    int addTransaction(const Transaction& transaction);   // → trả về ID mới (0 nếu lỗi)
    bool updateTransaction(const Transaction& transaction);
    bool deleteTransaction(int id);

    QVector<Transaction> getAllTransactions();
    Transaction getTransactionById(int id);
};
```

### 5.3 MVC Pattern (Model-View-Controller) — Cấu trúc Qt

**Mục đích:** Tách biệt 3 thành phần: **Model** (dữ liệu/Service), **View** (giao diện widget), **Controller** (slots xử lý sự kiện, tích hợp vào Page class). Trong Qt, Controller thường được tích hợp vào View thông qua cơ chế **Signals & Slots**.

**File mẫu:** `src/ui/pages/TransactionPage.h`

```cpp
// ============================================
// File: src/ui/pages/TransactionPage.h
// Pattern: MVC (View + Controller kết hợp trong Qt)
// ============================================
class TransactionPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionPage(QWidget* parent = nullptr);

private slots:
    // ★ Controller: Xử lý sự kiện từ giao diện
    void onAddTransaction();
    void onEditTransaction();
    void onDeleteTransaction();

private:
    void setupUI();    // Khởi tạo giao diện
    void loadData();   // Tải dữ liệu từ Service lên bảng

    // ★ View: Các widget giao diện
    QTableWidget* m_table;
    QPushButton*  m_btnAdd;
    // ...

    // ★ Model: thao tác qua AppContext → TransactionService
};
```

### 5.4 Inheritance & Polymorphism — Hệ thống Model

**Mục đích:** Thể hiện tính **kế thừa** và **đa hình** — yêu cầu cốt lõi của OOP. Tất cả Model đều kế thừa từ lớp cơ sở `BaseModel`.

**File:** `src/core/models/BaseModel.h`

```cpp
// ============================================
// File: src/core/models/BaseModel.h
// Pattern: Inheritance + Polymorphism
// ============================================
#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QString>

class BaseModel
{
protected:
    int m_id;

public:
    BaseModel() : m_id(0) {}
    explicit BaseModel(int id) : m_id(id) {}
    virtual ~BaseModel() = default;   // ★ Virtual destructor — BẮT BUỘC cho OOP

    int getId() const { return m_id; }
    void setId(int id) { m_id = id; }

    // ★ Pure virtual — buộc lớp con phải override (Đa hình)
    virtual QString toString() const = 0;

    // ★ Virtual — lớp con CÓ THỂ override
    virtual QString getDisplayName() const { return "BaseModel"; }
};

#endif // BASEMODEL_H
```

```cpp
// ============================================
// File: src/core/models/Transaction.h
// Kế thừa từ BaseModel
// ============================================
class Transaction : public BaseModel
{
private:
    int accountId;
    int categoryId;
    double amount;
    QDate transactionDate;
    QString note;
    QString type;        // "income" hoặc "expense" (chữ thường)

public:
    Transaction(int id, int accountId, int categoryId, double amount,
                const QDate& date, const QString& note, const QString& type);

    int getAccountId() const;
    int getCategoryId() const;
    double getAmount() const;
    QDate getDate() const;
    QString getNote() const;
    QString getType() const;

    // ★ Helper: nguồn chân lý về loại giao dịch
    bool isIncome() const { return type == "income"; }
    bool isExpense() const { return type == "expense"; }

    QString toString() const override;
    QString getDisplayName() const override { return note; }
};
```

**Minh họa Đa hình (Polymorphism) trong thực tế:**
```cpp
// Tạo mảng con trỏ BaseModel* — chứa nhiều loại đối tượng khác nhau
QVector<BaseModel*> items;
items.push_back(new Transaction(1, 1, 2, 50000, QDate(2026, 7, 24),
                                "Ăn trưa", "expense"));
items.push_back(new Category(1, "Ăn uống", CategoryType::Expense, "#ff5733", ""));

// ★ Đa hình: cùng gọi toString() nhưng kết quả khác nhau
for (const auto* item : items)
    qDebug() << item->toString();
// Transaction → "[CHI] 2026-07-24: 50000 VND - Ăn trưa"
// Category   → "Category [1]: Ăn uống"
```

### 5.5 Observer Pattern (Signals & Slots) — Giao tiếp giữa các thành phần

**Mục đích:** Khi dữ liệu thay đổi ở một nơi (ví dụ: thêm giao dịch), các nơi khác (Dashboard, Budget) tự động cập nhật mà không cần kiểm tra thủ công. Qt hỗ trợ sẵn qua cơ chế **Signals & Slots**.

**File:** `src/core/services/TransactionService.h`

```cpp
// ============================================
// File: src/core/services/TransactionService.h
// Pattern: Observer (Qt Signals & Slots)
// ============================================
class TransactionService : public QObject
{
    Q_OBJECT    // ★ Macro bắt buộc để dùng Signals & Slots

public:
    explicit TransactionService(QObject* parent = nullptr);

    // CRUD — tự cập nhật số dư account và phát signal
    bool addTransaction(const Transaction& transaction, QString* errorMessage = nullptr);
    bool updateTransaction(const Transaction& transaction, QString* errorMessage = nullptr);
    bool removeTransaction(int id, QString* errorMessage = nullptr);

    QVector<Transaction> getAllTransactions();

    double getTotalIncome();
    double getTotalExpense();
    double getBalance();

signals:
    // ★ Signals — phát ra khi dữ liệu thay đổi
    void transactionAdded(const Transaction& transaction);
    void transactionUpdated(const Transaction& transaction);
    void transactionRemoved(int transactionId);

private:
    TransactionRepository m_transactionRepo;
    AccountRepository m_accountRepo;
};
```

**Cách sử dụng Observer (kết nối Signal → Slot):**
```cpp
// Trong DashboardPage — tự động cập nhật khi có giao dịch mới
auto& txService = AppContext::instance().transactionService();

// ★ Kết nối: Khi transactionAdded() được phát, gọi loadData()
connect(&txService, &TransactionService::transactionAdded,
        this, &DashboardPage::loadData);

// Bây giờ, mỗi khi bất kỳ nơi nào gọi addTransaction(),
// DashboardPage sẽ TỰ ĐỘNG gọi loadData() để cập nhật giao diện!
```

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

> Chi tiết đầy đủ: [docs/coding-convention.md](docs/coding-convention.md)

### 6.1 Đặt tên biến & hàm

| Loại                | Quy tắc                | Ví dụ                         |
|:--------------------|:------------------------|:-------------------------------|
| Tên class           | `PascalCase`            | `TransactionService`, `DashboardPage` |
| Tên hàm / method    | `camelCase`             | `getAllTransactions()`, `addTransaction()` |
| Biến member (private)| `m_camelCase`          | `m_transactionRepo`, `m_accountRepo` |
| Biến local          | `camelCase`             | `totalIncome`, `errorMessage` |
| Tên file header     | `PascalCase.h`          | `Transaction.h`, `BudgetService.h` |
| Tên file source     | `PascalCase.cpp`        | `DatabaseManager.cpp`          |

### 6.2 Quy tắc chung

- ✅ Mỗi class nằm trong **một file .h riêng** (hoặc .h + .cpp nếu cần implementation)
- ✅ Comment bằng **tiếng Việt hoặc tiếng Anh** đều được, nhưng **nhất quán** trong từng file
- ✅ Dùng `const` khi biến không bị thay đổi
- ✅ Dùng `override` khi ghi đè hàm virtual
- ✅ **Chỉ CRUD qua Service layer**: thêm/sửa/xóa giao dịch phải qua `TransactionService`
  (để số dư account và signal được xử lý đúng), không gọi repository trực tiếp từ UI
- ✅ Loại giao dịch dùng chuỗi **chữ thường** `"income"`/`"expense"`, so sánh bằng
  `isIncome()/isExpense()`
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
[Test] Viết unit test cho TransactionService
[Fix] Sửa lỗi crash khi xóa giao dịch không tồn tại
[Docs] Cập nhật README với hướng dẫn chạy dự án
```

### 7.4 Quy trình ghép code (Merge) — Dành cho Gia Hưng

```bash
# ① Chuyển sang branch Hung
git checkout Hung

# ② Kéo code mới nhất của thành viên cần ghép
git pull origin AnKhang
git pull origin HuuLam

# ③ Merge từng branch vào Hung
git merge AnKhang
# → Nếu có CONFLICT: mở file bị conflict, sửa thủ công, rồi:
git add <file-bị-conflict>
git commit -m "[Merge] Ghép code AnKhang vào Hung"

# ④ Build & Test trên branch Hung
cmake --build build
./build/PersonalFinanceManager_Group10.app/Contents/MacOS/PersonalFinanceManager_Group10

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
| **An Khang** | Tạo `BaseModel.h` với virtual destructor & toString | `src/core/models/BaseModel.h`          | ✅ DONE     |
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
| **Hữu Lam**  | Viết `TransactionRepository` (CRUD giao dịch)      | `src/core/repositories/TransactionRepository.*` | ✅ DONE |
| **Hữu Lam**  | Viết `CategoryRepository` (CRUD danh mục)          | `src/core/repositories/CategoryRepository.*`    | ✅ DONE |
| **Hữu Lam**  | Viết `AccountRepository` (CRUD tài khoản)          | `src/core/repositories/AccountRepository.*`     | ✅ DONE |
| **Hữu Lam**  | Viết `BudgetRepository` (CRUD ngân sách)           | `src/core/repositories/BudgetRepository.*`      | ✅ DONE |
| **Minh Hạo**  | Viết `TransactionService` (thêm/xóa/sửa + signal)  | `src/core/services/TransactionService.*`     | ✅ DONE |
| **Minh Hạo**  | Viết `BudgetService` (kiểm tra ngân sách)          | `src/core/services/BudgetService.*`           | ✅ DONE |
| **Minh Hạo**  | Viết `ReportService` (thống kê thu/chi)            | `src/core/services/ReportService.*`           | ✅ DONE |
| **Minh Hạo**  | Viết `DateUtils.h` + `MoneyUtils.h`                | `src/utils/`                                  | ✅ DONE |

### Giai đoạn 3: Giao diện (UI) — Cần GĐ2 xong trước

| Người           | Công việc                                         | File                                     | Trạng thái |
|:----------------|:--------------------------------------------------|:-----------------------------------------|:----------:|
| **Việt Tường**  | Thiết kế `DashboardPage` (tổng quan tài chính)     | `src/ui/pages/DashboardPage.*`           | ✅ DONE     |
| **Việt Tường**  | Thiết kế `AccountPage` (quản lý tài khoản)         | `src/ui/pages/AccountPage.*`             | ✅ DONE     |
| **Việt Tường**  | Thiết kế `TransactionPage` (danh sách giao dịch)   | `src/ui/pages/TransactionPage.*`         | ✅ DONE     |
| **Việt Tường**  | Thiết kế `CategoryPage` (quản lý danh mục)         | `src/ui/pages/CategoryPage.*`            | ✅ DONE     |
| **Việt Tường**  | Thiết kế `BudgetPage` (quản lý ngân sách)          | `src/ui/pages/BudgetPage.*`              | ✅ DONE     |
| **Việt Tường**  | Thiết kế `ReportPage` (biểu đồ thống kê)           | `src/ui/pages/ReportPage.*`              | ✅ DONE     |
| **Việt Tường**  | Thiết kế các Dialog thêm/sửa (Account/Category/Transaction/Budget) | `src/ui/dialogs/*.cpp` | ✅ DONE     |
| **Việt Tường**  | Thiết kế `MainWindow` (sidebar + QStackedWidget)   | `src/ui/mainwindow.*`                    | ✅ DONE     |

### Giai đoạn 4: Ghép code & Kiểm thử — Song song với GĐ3

| Người          | Công việc                                          | File / Hành động                          | Trạng thái |
|:---------------|:---------------------------------------------------|:------------------------------------------|:----------:|
| **Gia Hưng**   | Ghép code GĐ1 + GĐ2 + GĐ3 vào branch Hung        | Merge branches                            | ✅ DONE     |
| **Gia Hưng**   | Cập nhật `CMakeLists.txt` thêm tất cả file .cpp     | `CMakeLists.txt`                          | ✅ DONE     |
| **Gia Hưng**   | Viết unit test cho `TransactionService`              | `tests/TransactionServiceTest.cpp`        | ✅ DONE     |
| **Gia Hưng**   | Viết unit test cho `BudgetService`                   | `tests/BudgetServiceTest.cpp`             | ✅ DONE     |
| **Gia Hưng**   | Build & chạy thử toàn bộ ứng dụng                   | Terminal / Qt Creator                     | ✅ DONE     |
| **Gia Hưng**   | Chuẩn bị slide & demo sản phẩm                      | Presentation                              | ⬜ TODO     |

---

## 9. 🧪 Hướng dẫn viết & chạy Unit Test

### 9.1 Cấu hình CMake cho Tests

`tests/CMakeLists.txt` định nghĩa 2 executable test. Mỗi test **link trực tiếp** các file
`.cpp` của service/repository/model mà nó sử dụng (không phụ thuộc target chính):

```cmake
# tests/CMakeLists.txt
find_package(Qt6 REQUIRED COMPONENTS Test)

qt_add_executable(TransactionServiceTest
    TransactionServiceTest.cpp
    ../src/core/services/TransactionService.cpp
    ../src/core/repositories/TransactionRepository.cpp
    ../src/core/repositories/AccountRepository.cpp
    ../src/core/repositories/CategoryRepository.cpp
    ../src/core/database/DatabaseManager.cpp
    # ... (models: Transaction.cpp, Account.cpp, Category.cpp, ...)
)

target_link_libraries(TransactionServiceTest PRIVATE
    Qt::Core
    Qt::Test
    Qt::Sql
)
```

Thêm dòng sau vào `CMakeLists.txt` ở thư mục gốc:

```cmake
add_subdirectory(tests)
```

### 9.2 Viết Unit Test với Qt Test

Test dùng DB tạm (`QTemporaryDir`) nên **không đụng** tới dữ liệu thật `data/finance.db`:

```cpp
// ============================================
// File: tests/TransactionServiceTest.cpp
// ============================================
#include <QtTest/QtTest>
#include <QTemporaryDir>
#include <QSqlQuery>
#include "../src/core/services/TransactionService.h"
#include "../src/core/repositories/AccountRepository.h"
#include "../src/core/database/DatabaseManager.h"

class TransactionServiceTest : public QObject
{
    Q_OBJECT

private:
    QTemporaryDir m_tempDir;
    TransactionService *service;

private slots:
    // ★ Chạy TRƯỚC tất cả test — mở DB tạm
    void initTestCase()
    {
        QVERIFY(m_tempDir.isValid());
        QVERIFY(DatabaseManager::instance().openDatabase(
            m_tempDir.filePath("test.db")));
        DatabaseManager::instance().initializeDatabase();
    }

    // ★ Chạy trước MỖI test — xóa dữ liệu để test độc lập
    void init()
    {
        QSqlQuery query;
        query.exec("DELETE FROM Transactions");
        query.exec("DELETE FROM Account");
        query.exec("DELETE FROM Category");
        service = new TransactionService();
    }

    // ★ Chạy SAU mỗi test
    void cleanup()
    {
        delete service;
    }

    void testAddTransaction()
    {
        // Tạo account + category hợp lệ (thỏa khóa ngoại)
        AccountRepository accountRepo;
        Account acc(0, "Cash", 0);
        accountRepo.addAccount(acc);

        Transaction tx(0, 1, 1, 100000, QDate(2026, 7, 10), "Lương", "income");

        QString errorMessage;
        QVERIFY(service->addTransaction(tx, &errorMessage));

        QCOMPARE(service->getAllTransactions().size(), 1);
    }

    void testTotalExpense()
    {
        // ...
        QCOMPARE(service->getTotalExpense(), 20000.0);
    }
};

QTEST_MAIN(TransactionServiceTest)
#include "TransactionServiceTest.moc"
```

> ⚠️ **Lưu ý:** Schema có `FOREIGN KEY` + `PRAGMA foreign_keys = ON`, nên test phải tạo
> `Account` và `Category` thật trước khi thêm `Transaction` để không vi phạm ràng buộc.

### 9.3 Chạy Unit Test

```bash
# Build tất cả (bao gồm tests)
cmake -B build
cmake --build build

# Chạy 2 bộ test
./build/tests/TransactionServiceTest
./build/tests/BudgetServiceTest
```

**Kết quả mong đợi:**
```
********* Start testing of TransactionServiceTest *********
PASS   : TransactionServiceTest::initTestCase()
PASS   : TransactionServiceTest::testAddTransaction()
PASS   : TransactionServiceTest::testValidate()
PASS   : TransactionServiceTest::testTotals()
PASS   : TransactionServiceTest::testUpdateTransaction()
PASS   : TransactionServiceTest::testRemoveTransaction()
PASS   : TransactionServiceTest::testSignalEmitted()
PASS   : TransactionServiceTest::cleanupTestCase()
Totals: 8 passed, 0 failed, 0 skipped
********* Finished testing of TransactionServiceTest *********

********* Start testing of BudgetServiceTest *********
PASS   : BudgetServiceTest::testAddBudget()
PASS   : BudgetServiceTest::testCalculateSpent()
PASS   : BudgetServiceTest::testIsOverBudget()
PASS   : BudgetServiceTest::testRemoveBudget()
Totals: 6 passed, 0 failed, 0 skipped
********* Finished testing of BudgetServiceTest *********
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

> 📅 **Cập nhật lần cuối:** 02/08/2026
> ✍️ **Tác giả:** Gia Hưng (Tester + Integration)
