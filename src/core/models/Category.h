#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>
#include "BaseModel.h"

enum class CategoryType
{
    Income,
    Expense
};

class Category : public BaseModel
{
private:
    int id;
    QString name;
    CategoryType type;
    QString color;
    QString icon;

public:
    Category();

    Category(int id,
             const QString& name,
             CategoryType type,
             const QString& color,
             const QString& icon);

    int getId() const;
    QString getName() const;
    CategoryType getType() const;
    QString getColor() const;
    QString getIcon() const;

    void setId(int id);
    void setName(const QString& name);
    void setType(CategoryType type);
    void setColor(const QString& color);
    void setIcon(const QString& icon);

    QString typeToString() const;
    static CategoryType stringToType(const QString &type);

    QString toString() const override;
};

#endif
