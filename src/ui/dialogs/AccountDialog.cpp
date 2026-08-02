#include "AccountDialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QVBoxLayout>

AccountDialog::AccountDialog(QWidget *parent) : QDialog(parent), m_id(0) {
  setWindowTitle("Thêm Tài Khoản");
  setMinimumWidth(380);

  QVBoxLayout *layout = new QVBoxLayout(this);
  QFormLayout *formLayout = new QFormLayout();

  nameEdit = new QLineEdit();
  nameEdit->setPlaceholderText("VD: Tiền mặt, Ngân hàng, Momo...");

  balanceSpin = new QDoubleSpinBox();
  balanceSpin->setRange(-999999999, 999999999);
  balanceSpin->setDecimals(0);
  balanceSpin->setSuffix(" VND");
  balanceSpin->setValue(0);

  descriptionEdit = new QLineEdit();
  descriptionEdit->setPlaceholderText("Mô tả (tùy chọn)");

  formLayout->addRow("Tên tài khoản:", nameEdit);
  formLayout->addRow("Số dư:", balanceSpin);
  formLayout->addRow("Mô tả:", descriptionEdit);

  QDialogButtonBox *buttonBox =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

  layout->addLayout(formLayout);
  layout->addWidget(buttonBox);

  setLayout(layout);
}

AccountDialog::AccountDialog(const Account &account, QWidget *parent)
    : QDialog(parent), m_id(account.getId()) {
  setWindowTitle("Sửa Tài Khoản");
  setMinimumWidth(380);

  QVBoxLayout *layout = new QVBoxLayout(this);
  QFormLayout *formLayout = new QFormLayout();

  nameEdit = new QLineEdit();
  nameEdit->setText(account.getName());

  balanceSpin = new QDoubleSpinBox();
  balanceSpin->setRange(-999999999, 999999999);
  balanceSpin->setDecimals(0);
  balanceSpin->setSuffix(" VND");
  balanceSpin->setValue(account.getBalance());

  descriptionEdit = new QLineEdit();
  descriptionEdit->setText(account.getDescription());

  formLayout->addRow("Tên tài khoản:", nameEdit);
  formLayout->addRow("Số dư:", balanceSpin);
  formLayout->addRow("Mô tả:", descriptionEdit);

  QDialogButtonBox *buttonBox =
      new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

  connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

  layout->addLayout(formLayout);
  layout->addWidget(buttonBox);

  setLayout(layout);
}

Account AccountDialog::getAccount() const {
  return Account(m_id, nameEdit->text(), balanceSpin->value(),
                 descriptionEdit->text());
}
