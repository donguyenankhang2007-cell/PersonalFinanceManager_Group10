#ifndef CATEGORY_H
#define CATEGORY_H

#include <string>
#include <stdexcept>

enum class CategoryType {
    Income,
    Expense
};

class Category {
public:
    // Ném std::invalid_argument nếu id <= 0 hoặc name rỗng
    Category(int id, const std::string& name, CategoryType type);

    int getId() const;
    std::string getName() const;
    CategoryType getType() const;

    void setName(const std::string& name); // validate name rỗng

    // So sánh 2 category theo id (dùng khi cần tìm/kiểm tra trùng trong danh sách)
    bool operator==(const Category& other) const;

private:
    int m_id;
    std::string m_name;
    CategoryType m_type;
};

#endif