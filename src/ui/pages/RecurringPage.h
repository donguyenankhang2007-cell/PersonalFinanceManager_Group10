#ifndef RECURRINGPAGE_H
#define RECURRINGPAGE_H

#include <QWidget>
#include <QTableWidget>

class QPushButton;

class RecurringPage : public QWidget
{
    Q_OBJECT

public:
    explicit RecurringPage(QWidget *parent = nullptr);

public slots:
    void loadRecurring();

private slots:
    void onAddRecurring();
    void onGenerateNow();

private:
    void editRecurringById(int id);
    void deleteRecurringById(int id);

    QTableWidget *table;
    QPushButton *btnAdd;
    QPushButton *btnGenerate;
};

#endif // RECURRINGPAGE_H
