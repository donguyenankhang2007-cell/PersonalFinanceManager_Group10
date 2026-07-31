#ifndef CATEGORYPAGE_H
#define CATEGORYPAGE_H

#include <QWidget>
#include <QListWidget>
#include <QPushButton>

class CategoryPage : public QWidget
{
    Q_OBJECT

public:
    explicit CategoryPage(QWidget *parent = nullptr);

public slots:
    void loadCategories();

private slots:
    void onAddCategory();
    void onDeleteCategory();

private:
    QListWidget *categoryList;
    QPushButton *btnAdd;
    QPushButton *btnDelete;
};

#endif // CATEGORYPAGE_H
