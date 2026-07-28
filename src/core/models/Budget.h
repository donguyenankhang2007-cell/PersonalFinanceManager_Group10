#ifndef BUDGET_H
#define BUDGET_H

#include <QString>
#include "BaseModel.h"

class Budget : public BaseModel
{
private:
    int id;
    int categoryId;
    double amount;
    int month;
    int year;

public:
    Budget();

    Budget(int id,
           int categoryId,
           double amount,
           int month,
           int year);

    ~Budget() = default;

    int getId() const;
    int getCategoryId() const;
    double getAmount() const;
    int getMonth() const;
    int getYear() const;

    void setId(int id);
    void setCategoryId(int categoryId);
    void setAmount(double amount);
    void setMonth(int month);
    void setYear(int year);

    QString toString() const override;
};

#endif // BUDGET_H
