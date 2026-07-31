#ifndef TRANSACTIONDIALOG_H
#define TRANSACTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QDateEdit>
#include <QVector>

#include "core/models/Transaction.h"
#include "core/models/Category.h"

class TransactionDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor cho THEM MOI
    explicit TransactionDialog(QWidget *parent = nullptr);

    // Constructor cho SUA (truyen Transaction hien tai de dien san form)
    TransactionDialog(const Transaction &transaction, QWidget *parent = nullptr);

    // Lay ket qua sau khi nguoi dung bam Luu
    Transaction getTransaction() const;

private slots:
    void onSaveClicked();

private:
    void setupUI();
    void loadCategories();
    void populateForm(const Transaction &transaction);

    // Cac truong nhap lieu
    QLineEdit  *m_amountEdit;
    QComboBox  *m_categoryCombo;
    QDateEdit  *m_dateEdit;
    QComboBox  *m_typeCombo;
    QLineEdit  *m_noteEdit;

    // Luu ID khi dang o che do sua
    int m_editId;
    int m_editAccountId;

    // Danh sach danh muc (de map ID <-> ten)
    QVector<Category> m_categories;
};

#endif // TRANSACTIONDIALOG_H
