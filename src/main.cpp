#include "mainwindow.h"
#include <QApplication>
#include <QtDebug>
#include <QSysInfo>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setOrganizationName("Aleksandr Hovhannisyan");
    app.setApplicationName("Scribe Text Editor");
    app.setOrganizationDomain("aleksandrhovhannisyan.com");

    MainWindow window;
    QApplication::setStyle("fusion");

    window.show();
    return app.exec();
}
