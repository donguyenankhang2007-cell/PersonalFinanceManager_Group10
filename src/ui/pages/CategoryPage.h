#ifndef CATEGORYPAGE_H
#define CATEGORYPAGE_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>

class CategoryPage : public QWidget
{
    Q_OBJECT

public:
    explicit CategoryPage(QWidget *parent = nullptr);

public slots:
    // Tai du lieu danh muc tu database
    void loadData();

private slots:
    void onAddClicked();
    void onEditClicked();
    void onDeleteClicked();

private:
    void setupUI();

    QTableWidget *m_table;
    QPushButton  *m_btnAdd;
    QPushButton  *m_btnEdit;
    QPushButton  *m_btnDelete;
};

#endif // CATEGORYPAGE_H
