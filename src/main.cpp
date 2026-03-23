#include "ui/views/msim_main.h"
#include <QApplication>
#include<QFile>
// #include "test/inst_word_tests.h"

int main(int argc, char *argv[]) {
  // run_tests();
#ifdef Q_OS_WIN
  qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");
#endif

#ifdef Q_OS_MAC
  qputenv("QT_APPEARANCE_STYLE", "Fusion");
  qputenv("QT_COLOR_SCHEME", "light");
#endif

  QApplication a(argc, argv);
  QFile file(":/amoled_style.qss");
  if(file.open(QFile::ReadOnly | QFile::Text)){
    QString style = file.readAll();
    a.setStyleSheet(style);
  }

  gui::views::msim_main w;
  w.show();
  return a.exec();
}

