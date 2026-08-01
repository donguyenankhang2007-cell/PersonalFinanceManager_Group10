#ifndef TRANSACTIONPAGE_H
#define TRANSACTIONPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

class TransactionPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionPage(QWidget *parent = nullptr);

public slots:
    void loadTransactions();

private slots:
    void onAddTransaction();
    void onDeleteTransaction();

private:
    QTableWidget *table;
    QPushButton *btnAdd;
    QPushButton *btnDelete;
};

#endif // TRANSACTIONPAGE_H
