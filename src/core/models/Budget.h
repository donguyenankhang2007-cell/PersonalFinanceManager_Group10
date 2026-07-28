#ifndef BUDGET_H
#define BUDGET_H

#include <QString>
#include "BaseModel.h"

class Budget : public BaseModel
{
private:
    int id;
    QString name;
    double limitAmount;
    double spentAmount;
    QString description;

public:
    Budget();

    Budget(int id,
           const QString& name,
           double limitAmount,
           double spentAmount = 0.0,
           const QString& description = "");

    ~Budget() = default;

    int getId() const;
    QString getName() const;
    double getLimitAmount() const;
    double getSpentAmount() const;
    QString getDescription() const;

    void setId(int id);
    void setName(const QString& name);
    void setLimitAmount(double limitAmount);
    void setSpentAmount(double spentAmount);
    void setDescription(const QString& description);

    QString toString() const override;
};

#endif // BUDGET_H
