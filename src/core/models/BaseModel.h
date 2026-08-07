#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QString>

class BaseModel
{
public:
    BaseModel()
        : m_id(0)
    {
    }

    explicit BaseModel(int id)
        : m_id(id)
    {
    }

    virtual ~BaseModel() = default;

    int getId() const { return m_id; }
    void setId(int id) { m_id = id; }

    virtual QString toString() const = 0;
    virtual QString getDisplayName() const { return "BaseModel"; }

protected:
    int m_id;
};

#endif // BASEMODEL_H
