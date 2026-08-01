#ifndef ACCOUNTPAGE_H
#define ACCOUNTPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

class AccountPage : public QWidget
{
    Q_OBJECT

public:
    explicit AccountPage(QWidget *parent = nullptr);

public slots:
    void loadAccounts();

private slots:
    void onAddAccount();
    void onDeleteAccount();

private:
    QTableWidget *table;
    QPushButton *btnAdd;
    QPushButton *btnDelete;
};

#endif // ACCOUNTPAGE_H
