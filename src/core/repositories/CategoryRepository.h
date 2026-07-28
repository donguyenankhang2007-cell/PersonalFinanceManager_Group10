#ifndef CATEGORYREPOSITORY_H
#define CATEGORYREPOSITORY_H

#include <QVector>

#include "../models/Category.h"

class CategoryRepository
{
public:
    CategoryRepository();

    bool addCategory(const Category &category);

    bool updateCategory(const Category &category);

    bool deleteCategory(int id);

    QVector<Category> getAllCategories();

    Category getCategoryById(int id);

};

#endif // CATEGORYREPOSITORY_H