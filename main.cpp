#include "Mainwindow.h"
#include <QApplication>
#include "mserver.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    mserver Server = mserver();
    return a.exec();
}
