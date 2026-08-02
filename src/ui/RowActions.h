// ============================================
// File: src/ui/RowActions.h
// Mo ta: Tao widget chua 2 nut icon Edit/Delete
//        cho moi hang cua bang (thay cho nut tren dau trang).
// ============================================
#ifndef ROWACTIONS_H
#define ROWACTIONS_H

#include <QWidget>
#include <functional>

class RowActions
{
public:
    // Tạo 2 nút icon Edit/Delete cho 1 hàng có id = rowId.
    // editFn/deleteFn được gọi kèm rowId khi bấm.
    static QWidget* create(int rowId,
                           std::function<void(int)> editFn,
                           std::function<void(int)> deleteFn,
                           QWidget* parent = nullptr);
};

#endif // ROWACTIONS_H
