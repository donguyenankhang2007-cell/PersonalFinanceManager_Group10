#ifndef CATEGORYPAGE_H
#define CATEGORYPAGE_H

#include <QWidget>
#include <QTableWidget>

class QPushButton;

class CategoryPage : public QWidget
{
    Q_OBJECT

public:
    explicit CategoryPage(QWidget *parent = nullptr);

public slots:
    void loadCategories();

private slots:
    void onAddCategory();

private:
    void editCategoryById(int id);
    void deleteCategoryById(int id);

    QTableWidget *table;
    QPushButton *btnAdd;
    class QLabel *titleLabel;
    class QLabel *subtitleLabel;
};

#endif // CATEGORYPAGE_H
