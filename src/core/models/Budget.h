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
<<<<<<< HEAD
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
=======
           int categoryId,
           double amount,
           int month,
           int year);

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
>>>>>>> Lam
};

#endif // BUDGET_H
