#ifndef CATEGORYDIALOG_H
#define CATEGORYDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>

#include "core/models/Category.h"

class CategoryDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor cho THEM MOI
    explicit CategoryDialog(QWidget *parent = nullptr);

    // Constructor cho SUA (truyen Category hien tai)
    CategoryDialog(const Category &category, QWidget *parent = nullptr);

    // Lay ket qua sau khi nguoi dung bam Luu
    Category getCategory() const;

private slots:
    void onSaveClicked();

private:
    void setupUI();
    void populateForm(const Category &category);

    // Cac truong nhap lieu
    QLineEdit *m_nameEdit;
    QComboBox *m_typeCombo;
    QLineEdit *m_colorEdit;
    QLineEdit *m_iconEdit;

    // Luu ID khi dang o che do sua
    int m_editId;
};

#endif // CATEGORYDIALOG_H
