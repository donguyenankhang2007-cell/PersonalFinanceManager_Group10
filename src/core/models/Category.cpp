#include "Category.h"

Category::Category()
    : BaseModel(),
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
    : BaseModel(id),
      name(name),
      type(type),
      color(color),
      icon(icon)
{
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
    if (type.compare("income", Qt::CaseInsensitive) == 0)
        return CategoryType::Income;

    return CategoryType::Expense;
}

QString Category::toString() const
{
    return QString("Category [%1] %2 | Type: %3")
            .arg(m_id)
            .arg(name)
            .arg(typeToString());
}
