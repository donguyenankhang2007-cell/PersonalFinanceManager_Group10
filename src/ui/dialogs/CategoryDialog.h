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
    explicit CategoryDialog(QWidget *parent = nullptr);

    Category getCategory() const;

private:
    QLineEdit *nameEdit;
    QComboBox *typeCombo;
    QLineEdit *colorEdit;
    QLineEdit *iconEdit;
};

#endif // CATEGORYDIALOG_H
