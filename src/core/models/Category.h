#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>

class Category
{
private:
    int id;
    QString name;
    QString type;
    QString description;

public:
    Category();

    Category(int id,
             const QString& name,
             const QString& type,
             const QString& description = "");

    ~Category() = default;

    int getId() const;
    QString getName() const;
    QString getType() const;
    QString getDescription() const;

    void setId(int id);
    void setName(const QString& name);
    void setType(const QString& type);
    void setDescription(const QString& description);

    QString toString() const;
};

#endif // CATEGORY_H
