#ifndef CATEGORYDIALOG_H
#define CATEGORYDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>

#include "../../core/models/Category.h"

class CategoryDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor cho THEM MOI
    explicit CategoryDialog(QWidget *parent = nullptr);

    // Constructor cho SUA (pre-fill du lieu)
    CategoryDialog(const Category& category, QWidget *parent = nullptr);

    Category getCategory() const;

private:
    int m_id;
    QLineEdit *nameEdit;
    QComboBox *typeCombo;
    QLineEdit *colorEdit;
    QLineEdit *iconEdit;
};

#endif // CATEGORYDIALOG_H
