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
    explicit AccountDialog(QWidget *parent = nullptr);

    Account getAccount() const;

private:
    QLineEdit *nameEdit;
    QDoubleSpinBox *balanceSpin;
    QLineEdit *descriptionEdit;
};

#endif // ACCOUNTDIALOG_H
