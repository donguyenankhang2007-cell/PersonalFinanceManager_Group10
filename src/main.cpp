#include "app/AppContext.h"
#include "core/database/DatabaseManager.h"
#include "ui/mainwindow.h"

#include "core/models/Account.h"
#include "core/repositories/AccountRepository.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);

  // Font chữ hiện đại
  QFont font("Helvetica Neue");
  font.setStyleHint(QFont::SansSerif);
  font.setPixelSize(14); // 14px base font size for readability
  QApplication::setFont(font);

  DatabaseManager::instance().openDatabase();
  DatabaseManager::instance().initializeDatabase();

  // Sinh các giao dịch định kỳ đến hạn khi khởi động
  AppContext::instance()
      .recurringTransactionService()
      .generateDueTransactions();

  MainWindow w;
  w.show();

  return a.exec();
}