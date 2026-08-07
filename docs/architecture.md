# Kiến trúc hệ thống

## Tổng quan

Personal Finance Manager là ứng dụng desktop quản lý tài chính cá nhân viết bằng
**C++17 + Qt 6 (Widgets) + SQLite**, tổ chức theo mô hình **Layered Architecture**
kết hợp **Repository** và **Service**:

```
┌─────────────────────────────────────────────┐
│  UI (Widgets)                               │
│  mainwindow · pages · dialogs               │
│            │                                │
│            ▼                                │
│  AppContext (singleton, khởi tạo service)   │
│            │                                │
│            ▼                                │
│  Service layer (logic nghiệp vụ + signals)  │
│  TransactionService · BudgetService         │
│  ReportService                              │
│            │                                │
│            ▼                                │
│  Repository layer (thao tác SQLite)         │
│  AccountRepository · CategoryRepository     │
│  TransactionRepository · BudgetRepository   │
│            │                                │
│            ▼                                │
│  DatabaseManager (kết nối + schema SQLite)  │
└─────────────────────────────────────────────┘
```

## Các tầng

### 1. UI — `src/ui/`

- `mainwindow.cpp/.h`: cửa sổ chính, thanh điều hướng bên trái (sidebar) + `QStackedWidget`.
  Toàn bộ giao diện được dựng bằng code (không dùng file `.ui`).
- `pages/`: các trang `DashboardPage`, `AccountPage`, `CategoryPage`,
  `TransactionPage`, `BudgetPage`, `ReportPage`. Trang nào có dữ liệu động đều có
  phương thức `loadData()` gọi khi được chuyển tới và khi nhận signal từ service.
- `dialogs/`: hộp thoại thêm/sửa `AccountDialog`, `CategoryDialog`,
  `TransactionDialog`, `BudgetDialog`. Mỗi dialog có 2 constructor:
  - `Dialog(QWidget*)` — chế độ thêm mới.
  - `Dialog(const Model&, QWidget*)` — chế độ sửa (điền sẵn dữ liệu).

### 2. AppContext — `src/app/AppContext.h`

Singleton cung cấp quyền truy cập tới toàn bộ service/repository cho tầng UI.
`database()` trả về `DatabaseManager::instance()` (chỉ có **một** kết nối DB trong app).

### 3. Service — `src/core/services/`

Lớp nghiệp vụ; là **cửa ngõ duy nhất** mà UI dùng để CRUD:

- `TransactionService`: `QObject` phát signal `transactionAdded/Updated/Removed`.
  Khi thêm/sửa/xóa giao dịch, service **tự động cập nhật số dư account** và validate
  dữ liệu đầu vào (trả `QString* errorMessage` khi lỗi).
- `BudgetService`: `QObject` phát signal `budgetAdded/Updated/Removed`.
  `calculateSpent()` tính tổng chi tiêu theo `categoryId` + tháng/năm của budget.
- `ReportService`: hàm thuần túy tính tổng thu/chi, số dư, so sánh chi tiêu
  giữa 2 tháng.

### 4. Repository — `src/core/repositories/`

Encapsulate toàn bộ câu lệnh SQL của từng entity. UI **không bao giờ** gọi repository
trực tiếp (ngoại trừ những hàm đọc đơn giản trong `AppContext` khi cần danh sách).

### 5. Database — `src/core/database/DatabaseManager.h`

Singleton quản lý kết nối `QSqlDatabase`:
- `openDatabase()` — mở file `data/finance.db` (tự tìm thư mục `data/`).
- `openDatabase(path)` — mở DB tại đường dẫn tùy chọn (dùng trong unit test).
- `initializeDatabase()` — đọc schema từ qrc → file đĩa → SQL nhúng (3 nguồn dự phòng).
- Bật `PRAGMA foreign_keys = ON` để đảm bảo toàn vẹn tham chiếu.

## Mô hình dữ liệu — `src/core/models/`

- `BaseModel`: lớp cơ sở chứa `m_id` + `getId()/setId()/getDisplayName()`.
- `Account`: tài khoản tiền, có `deposit()/withdraw()` đổi số dư.
- `Category`: danh mục thu/chi (`CategoryType::Income/Expense`).
  `typeToString()` trả `"income"`/`"expense"` (chữ thường) để đồng bộ với `Transaction`.
- `Transaction`: giao dịch, `type` là `"income"` hoặc `"expense"` (chữ thường).
  `isIncome()/isExpense()` là nguồn chân lý về loại giao dịch.
- `Budget`: hạn mức chi tiêu theo `categoryId` + `month` + `year`.
- `Report`: kết quả tổng hợp thu/chi.

## Luồng xử lý điển hình

**Thêm giao dịch (thu/chi):**
1. UI gọi `TransactionService::addTransaction(tx, &error)`.
2. Service validate; nếu lỗi trả về `false` + gán `error`.
3. `TransactionRepository::addTransaction()` chèn DB, trả về id mới.
4. `applyAccountBalance()` cộng/trừ số dư account tương ứng.
5. Service phát signal `transactionAdded` → các trang Dashboard/Budget tự làm mới.

## Design patterns

| Pattern | Ứng dụng |
|---|---|
| **Singleton** | `AppContext`, `DatabaseManager` |
| **Repository** | mỗi entity 1 repository thao tác SQLite |
| **Observer** | `TransactionService`/`BudgetService` phát signal; các page đăng ký lắng nghe và cập nhật UI |
| **Factory-ish** | `Category::stringToType`/`typeToString` chuẩn hóa kiểu dữ liệu |

## Unit test — `tests/`

- `TransactionServiceTest` — CRUD giao dịch, validate, tổng thu/chi, cập nhật số dư,
  phát signal. Dùng DB tạm `QTemporaryDir` (không đụng dữ liệu thật).
- `BudgetServiceTest` — CRUD budget, tính chi tiêu theo tháng, vượt hạn mức.
- Chạy: `ctest` hoặc chạy trực tiếp 2 binary trong `build/.../tests/`.
