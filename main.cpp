#include "Mainwindow.h"
#include <QApplication>
#include "iostream"
#include "ServerManager.h"

#include <fstream>
#include <string>
#include "interpreter/Lexer.h"
#include "interpreter/Parser.h"
#include "interpreter/Lexer.cpp"
#include "interpreter/Parser.cpp"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
