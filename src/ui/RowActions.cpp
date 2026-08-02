// ============================================
// File: src/ui/RowActions.cpp
// ============================================
#include "RowActions.h"

#include <QHBoxLayout>
#include <QPushButton>
#include <QIcon>

QWidget* RowActions::create(int rowId,
                            std::function<void(int)> editFn,
                            std::function<void(int)> deleteFn,
                            QWidget* parent)
{
    QWidget *container = new QWidget(parent);
    container->setAttribute(Qt::WA_TranslucentBackground);

    QHBoxLayout *layout = new QHBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    QPushButton *btnEdit = new QPushButton();
    btnEdit->setObjectName("rowEditBtn");
    btnEdit->setIcon(QIcon(":/icons/edit.svg"));
    btnEdit->setIconSize(QSize(16, 16));
    btnEdit->setFixedSize(24, 24);
    btnEdit->setCursor(Qt::PointingHandCursor);
    btnEdit->setToolTip("Edit");

    QPushButton *btnDelete = new QPushButton();
    btnDelete->setObjectName("rowDeleteBtn");
    btnDelete->setIcon(QIcon(":/icons/delete.svg"));
    btnDelete->setIconSize(QSize(16, 16));
    btnDelete->setFixedSize(24, 24);
    btnDelete->setCursor(Qt::PointingHandCursor);
    btnDelete->setToolTip("Delete");

    QObject::connect(btnEdit, &QPushButton::clicked,
                     [editFn, rowId]() { editFn(rowId); });
    QObject::connect(btnDelete, &QPushButton::clicked,
                     [deleteFn, rowId]() { deleteFn(rowId); });

    layout->addStretch();
    layout->addWidget(btnEdit);
    layout->addWidget(btnDelete);
    layout->addStretch();

    return container;
}
