#include "ui/mainwindow.h"
#include "core/database/DatabaseManager.h"

#include "core/repositories/AccountRepository.h"
#include "core/models/Account.h"

#include <QApplication>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    DatabaseManager::instance().openDatabase();
    DatabaseManager::instance().initializeDatabase();

    MainWindow w;
    w.show();

    return a.exec();
}