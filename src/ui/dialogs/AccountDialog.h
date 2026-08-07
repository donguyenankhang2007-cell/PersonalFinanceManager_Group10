#ifndef ACCOUNTDIALOG_H
#define ACCOUNTDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QDoubleSpinBox>

#include "../../core/models/Account.h"

class AccountDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor cho THEM MOI
    explicit AccountDialog(QWidget *parent = nullptr);

    // Constructor cho SUA (pre-fill du lieu)
    AccountDialog(const Account& account, QWidget *parent = nullptr);

    Account getAccount() const;

private:
    int m_id;
    QLineEdit *nameEdit;
    QDoubleSpinBox *balanceSpin;
    QLineEdit *descriptionEdit;
};

#endif // ACCOUNTDIALOG_H
