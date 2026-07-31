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
    // Tai du lieu giao dich tu database len bang
    void loadData();

private slots:
    void onAddClicked();
    void onEditClicked();
    void onDeleteClicked();

private:
    void setupUI();

    QTableWidget *m_table;
    QPushButton  *m_btnAdd;
    QPushButton  *m_btnEdit;
    QPushButton  *m_btnDelete;
};

#endif // TRANSACTIONPAGE_H
