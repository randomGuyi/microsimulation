
#include <QApplication>
#include "ui/views/msim_main.h"
#include "test/inst_word_tests.h"

int main(int argc, char *argv[])
{
   run_tests();
#ifdef Q_OS_WIN
    // Forces the Windows plugin with darkmode=0 on Windows
    qputenv("QT_QPA_PLATFORM", "windows:darkmode=0");
#endif

#ifdef Q_OS_MAC
    // Forces the Light appearance style on macOS
    // Qt looks for 'fusion', 'windows', 'macintosh', etc.
    // However, on macOS, setting the environment variable specific to the color scheme is usually more reliable.
    // The following environment variable forces the native Light style:
    qputenv("QT_APPEARANCE_STYLE", "Fusion");

    // OR, sometimes setting the color scheme preference is needed (less common for Qt 6):
    qputenv("QT_COLOR_SCHEME", "light");
#endif

    QApplication a(argc, argv);

    // ... rest of your code ...
    msim_main w;
    w.show();
    return a.exec();
}
