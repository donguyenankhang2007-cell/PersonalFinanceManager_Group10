# Quy ước lập trình

Áp dụng cho toàn bộ mã nguồn của dự án **Personal Finance Manager**
(C++17, Qt 6 Widgets, SQLite).

## Đặt tên

| Thành phần | Quy tắc | Ví dụ |
|---|---|---|
| Lớp | `PascalCase` | `TransactionService`, `AccountRepository` |
| Hàm | `camelCase`, có tiền tố mô tả hành động | `getAllTransactions()`, `addAccount()` |
| Biến thành viên | `m_` + `camelCase` | `m_accountRepo`, `m_tempDir` |
| Tham số | `camelCase` | `const Transaction& transaction` |
| Biến cục bộ | `camelCase` | `errorMessage`, `totalIncome` |
| Macro/guard | `UPPER_SNAKE_CASE` | `TRANSACTIONSERVICE_H` |
| File | trùng tên lớp | `TransactionService.h/.cpp` |

## Cấu trúc file header

```
#ifndef CLASSNAME_H
#define CLASSNAME_H

#include <Qt khai báo cần thiết>
#include "../models/... (include nội bộ)

class ClassName
{
public:
    // constructor, method public

signals:        // (nếu là QObject)
    ...

private:
    // method private, biến thành viên m_
};

#endif // CLASSNAME_H
```

- Header chỉ khai báo, định nghĩa đặt trong `.cpp`.
- Hàm ngắn (getter) có thể định nghĩa inline trong header.
- Dùng guard `#ifndef` (không dùng `#pragma once`).

## Ngôn ngữ

- Mã nguồn và comment viết bằng **tiếng Việt (không dấu)** hoặc tiếng Anh,
  thống nhất trong cùng 1 file. Không chèn tiếng Việt có dấu vào chuỗi code trực tiếp.
- String hiển thị cho người dùng có thể dùng tiếng Việt (có dấu) qua UTF-8.

## Quy tắc chung

- **Chỉ sửa qua service layer cho logic nghiệp vụ**: không gọi
  `TransactionRepository` trực tiếp từ UI khi thêm/sửa/xóa giao dịch — phải qua
  `TransactionService` (để số dư account và signal được xử lý đúng).
- **Số dư account không được sửa tay** ở UI; mọi thay đổi đến từ
  `TransactionService::applyAccountBalance()`.
- **Loại giao dịch dùng chuỗi `"income"`/`"expense"` chữ thường**. Khi so sánh,
  dùng `Transaction::isIncome()/isExpense()` thay vì so sánh chuỗi tùy ý.
- Phân tách rõ 3 trách nhiệm: UI (không có logic SQL) → Service (logic nghiệp vụ)
  → Repository (SQL).
- UI tự cập nhật qua **signal của service** (Observer), không gọi lại `loadData()`
  rải rác.

## Xử lý lỗi

- Hàm ghi dữ liệu trả `bool`; lý do lỗi truyền qua tham số
  `QString* errorMessage = nullptr` (out param), UI hiển thị bằng `QMessageBox`.
- Không dùng `std::cerr`/`qDebug()` để thông báo lỗi cho người dùng.

## Style

- Indent 4 dấu cách (không dùng tab).
- Mở `{` cùng dòng với khai báo hàm (`Allman` không bắt buộc; ưu tiên thống nhất
  theo file hiện có).
- Mỗi câu lệnh độc lập 1 dòng, có `;` đầy đủ.
- Biến `const` khi không thay đổi giá trị.

## Kiểm thử

- Mỗi service quan trọng có unit test trong `tests/`.
- Test phải chạy độc lập: dùng `QTemporaryDir` + `DatabaseManager::openDatabase(path)`
  và xóa dữ liệu giữa các test case để không phụ thuộc trạng thái lẫn nhau.
- Trước khi hoàn thành 1 thay đổi, phải build và chạy toàn bộ test:
  - `cmake --build build/Qt_6_11_1_for_macOS_Debug`
  - chạy `tests/TransactionServiceTest` và `tests/BudgetServiceTest`
