#ifndef BASEMODEL_H
#define BASEMODEL_H

#include <QString>

class BaseModel
{
public:
    BaseModel() = default;

    virtual ~BaseModel() = default;

    virtual QString toString() const = 0;
};

#endif 