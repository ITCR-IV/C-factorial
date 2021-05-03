#include "Mainwindow.h"
#include <QApplication>
#include <iostream>
#include "ServerManager.h"

#include <fstream>
#include <string>
#include "interpreter/Lexer.h"
#include "interpreter/Parser.h"
#include "interpreter/Lexer.cpp"
#include "interpreter/Parser.cpp"
#include "server/JsonEncoder.h"
#include "unistd.h"

using namespace std;

int main(int argc, char *argv[])
{
    ServerManager *manager = ServerManager::getInstance(9999);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
