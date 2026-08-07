#ifndef ACCOUNTPAGE_H
#define ACCOUNTPAGE_H

#include <QWidget>
#include <QTableWidget>

class QPushButton;

class AccountPage : public QWidget
{
    Q_OBJECT

public:
    explicit AccountPage(QWidget *parent = nullptr);

public slots:
    void loadAccounts();

private slots:
    void onAddAccount();

private:
    void editAccountById(int id);
    void deleteAccountById(int id);

    QTableWidget *m_table;
    QPushButton *m_btnAdd;
    class QLabel *m_titleLabel;
    class QLabel *m_subtitleLabel;
};

#endif // ACCOUNTPAGE_H
