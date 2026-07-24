#include "Category.h"

Category::Category(int id, const std::string& name, CategoryType type)
    : m_id(id), m_name(name), m_type(type) {
    if (id <= 0) {
        throw std::invalid_argument("Id phai lon hon 0");
    }
    if (name.empty()) {
        throw std::invalid_argument("Ten category khong duoc de trong");
    }
}

int Category::getId() const { return m_id; }
std::string Category::getName() const { return m_name; }
CategoryType Category::getType() const { return m_type; }

void Category::setName(const std::string& name) {
    if (name.empty()) {
        throw std::invalid_argument("Ten category khong duoc de trong");
    }
    m_name = name;
}

bool Category::operator==(const Category& other) const {
    return m_id == other.m_id;
}