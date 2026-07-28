#include "Category.h"

Category::Category()
    : id(0),
    name(""),
    type(CategoryType::Expense),
    color(""),
    icon("")
{
}

Category::Category(int id,
                   const QString& name,
                   CategoryType type,
                   const QString& color,
                   const QString& icon)
    : id(id),
    name(name),
    type(type),
    color(color),
    icon(icon)
{
}

int Category::getId() const
{
    return id;
}

QString Category::getName() const
{
    return name;
}

CategoryType Category::getType() const
{
    return type;
}

QString Category::getColor() const
{
    return color;
}

QString Category::getIcon() const
{
    return icon;
}

void Category::setId(int id)
{
    this->id = id;
}

void Category::setName(const QString& name)
{
    this->name = name;
}

void Category::setType(CategoryType type)
{
    this->type = type;
}

void Category::setColor(const QString& color)
{
    this->color = color;
}

void Category::setIcon(const QString& icon)
{
    this->icon = icon;
}

QString Category::typeToString() const
{
    return (type == CategoryType::Income)
               ? "Income"
               : "Expense";
}

CategoryType Category::stringToType(const QString &type)
{
    if (type == "Income")
        return CategoryType::Income;

    return CategoryType::Expense;
}