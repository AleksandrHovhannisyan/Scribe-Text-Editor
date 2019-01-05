#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    QApplication::setStyle("fusion");

    window.show();
    return app.exec();
}
