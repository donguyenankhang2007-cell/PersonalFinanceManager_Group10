#include "Category.h"

Category::Category()
    : id(0),
      name(""),
      type(""),
      description("")
{
}

Category::Category(int id,
                   const QString& name,
                   const QString& type,
                   const QString& description)
    : id(id),
      name(name),
      type(type),
      description(description)
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

QString Category::getType() const
{
    return type;
}

QString Category::getDescription() const
{
    return description;
}

void Category::setId(int id)
{
    this->id = id;
}

void Category::setName(const QString& name)
{
    this->name = name;
}

void Category::setType(const QString& type)
{
    this->type = type;
}

void Category::setDescription(const QString& description)
{
    this->description = description;
}

QString Category::toString() const
{
    return QString("%1 (%2)")
            .arg(name)
            .arg(type);
}