#include "CategoryRepository.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

#include "../database/DatabaseManager.h"

CategoryRepository::CategoryRepository()
{
}

bool CategoryRepository::addCategory(const Category &category)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "INSERT INTO Category(name, type, color, icon) "
        "VALUES(:name, :type, :color, :icon)");

    query.bindValue(":name", category.getName());
    query.bindValue(":type", category.typeToString());
    query.bindValue(":color", category.getColor());
    query.bindValue(":icon", category.getIcon());

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

bool CategoryRepository::updateCategory(const Category &category)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "UPDATE Category "
        "SET name = :name, "
        "type = :type, "
        "color = :color, "
        "icon = :icon "
        "WHERE id = :id");

    query.bindValue(":id", category.getId());
    query.bindValue(":name", category.getName());
    query.bindValue(":type", category.typeToString());
    query.bindValue(":color", category.getColor());
    query.bindValue(":icon", category.getIcon());

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

bool CategoryRepository::deleteCategory(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "DELETE FROM Category WHERE id = :id");

    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

QVector<Category> CategoryRepository::getAllCategories()
{
    QVector<Category> categories;

    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare("SELECT * FROM Category");

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        return categories;
    }

    while (query.next())
    {
        Category category(
            query.value("id").toInt(),
            query.value("name").toString(),
            Category::stringToType(query.value("type").toString()),
            query.value("color").toString(),
            query.value("icon").toString());

        categories.push_back(category);
    }

    return categories;
}

Category CategoryRepository::getCategoryById(int id)
{
    QSqlQuery query(DatabaseManager::instance().database());

    query.prepare(
        "SELECT * FROM Category WHERE id = :id");

    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << query.lastError().text();
        return Category();
    }

    if (query.next())
    {
        return Category(
            query.value("id").toInt(),
            query.value("name").toString(),
            Category::stringToType(query.value("type").toString()),
            query.value("color").toString(),
            query.value("icon").toString());
    }

    return Category();
}

