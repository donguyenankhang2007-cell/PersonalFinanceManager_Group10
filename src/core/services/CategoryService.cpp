#include "CategoryService.h"

CategoryService::CategoryService(QObject* parent) : QObject(parent)
{
}

bool CategoryService::addCategory(const Category& category, QString* errorMessage)
{
    if (category.getName().isEmpty()) {
        if (errorMessage) *errorMessage = "Tên danh mục không được để trống!";
        return false;
    }
    if (m_categoryRepo.addCategory(category)) {
        emit categoryAdded(category);
        return true;
    }
    if (errorMessage) *errorMessage = "Lỗi cơ sở dữ liệu khi thêm danh mục!";
    return false;
}

bool CategoryService::updateCategory(const Category& category, QString* errorMessage)
{
    if (category.getName().isEmpty()) {
        if (errorMessage) *errorMessage = "Tên danh mục không được để trống!";
        return false;
    }
    if (m_categoryRepo.updateCategory(category)) {
        emit categoryUpdated(category);
        return true;
    }
    if (errorMessage) *errorMessage = "Lỗi cơ sở dữ liệu khi cập nhật danh mục!";
    return false;
}

bool CategoryService::removeCategory(int id, QString* errorMessage)
{
    if (m_categoryRepo.deleteCategory(id)) {
        emit categoryRemoved(id);
        return true;
    }
    if (errorMessage) *errorMessage = "Lỗi cơ sở dữ liệu khi xóa danh mục! Có thể danh mục đang được sử dụng.";
    return false;
}

QVector<Category> CategoryService::getAllCategories()
{
    return m_categoryRepo.getAllCategories();
}

Category CategoryService::getCategoryById(int id)
{
    return m_categoryRepo.getCategoryById(id);
}
