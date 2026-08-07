#ifndef TRANSACTIONPAGE_H
#define TRANSACTIONPAGE_H

#include <QWidget>
#include <QTableWidget>

class QPushButton;
class QComboBox;
class QLineEdit;
class QDateEdit;
class QCheckBox;

class TransactionPage : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionPage(QWidget *parent = nullptr);

public slots:
    void loadTransactions();

private slots:
    void onAddTransaction();
    void applyFilters();
    void clearFilters();

private:
    void editTransactionById(int id);
    void deleteTransactionById(int id);

    QTableWidget *table;
    QPushButton *btnAdd;

    QComboBox *filterType;
    QComboBox *filterAccount;
    QComboBox *filterCategory;
    QLineEdit *searchEdit;
    QCheckBox *periodCheck;
    QDateEdit *fromDate;
    QDateEdit *toDate;
    QPushButton *btnClear;
};

#endif // TRANSACTIONPAGE_H
