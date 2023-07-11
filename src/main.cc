#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[]) {
  QApplication application(argc, argv);
  QApplication::setStyle("fusion");
  MainWindow main_window;
  main_window.show();
  return QApplication::exec();
}
