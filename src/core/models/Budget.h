#ifndef BUDGET_H
#define BUDGET_H

class Budget
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
};

#endif // BUDGET_H
