#include <QApplication>
#include "OptimizationWindow.h"  // Подключаем файл с определением окна

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    OptimizationWindow window;
    window.setWindowTitle("Optimization Interface");
    window.showMaximized();

    return app.exec();
}
