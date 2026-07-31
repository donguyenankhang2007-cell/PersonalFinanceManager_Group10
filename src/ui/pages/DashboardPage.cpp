#include "DashboardPage.h"

#include <QVBoxLayout>
#include <QLabel>

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent)
{

    QVBoxLayout *layout = new QVBoxLayout(this);

    QLabel *title =
        new QLabel("Dashboard");

    layout->addWidget(title);

    setLayout(layout);

}
