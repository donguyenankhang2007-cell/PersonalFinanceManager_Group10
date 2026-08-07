#ifndef CATEGORYSERVICE_H
#define CATEGORYSERVICE_H

#include <QObject>
#include <QVector>
#include "../models/Category.h"
#include "../repositories/CategoryRepository.h"

class CategoryService : public QObject
{
    Q_OBJECT

public:
    explicit CategoryService(QObject* parent = nullptr);

    bool addCategory(const Category& category, QString* errorMessage = nullptr);
    bool updateCategory(const Category& category, QString* errorMessage = nullptr);
    bool removeCategory(int id, QString* errorMessage = nullptr);

    QVector<Category> getAllCategories();
    Category getCategoryById(int id);

signals:
    void categoryAdded(const Category& category);
    void categoryUpdated(const Category& category);
    void categoryRemoved(int categoryId);

private:
    CategoryRepository m_categoryRepo;
};

#endif // CATEGORYSERVICE_H
