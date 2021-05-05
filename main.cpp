#include "Mainwindow.h"
#include <QApplication>
#include <iostream>
#include "ServerManager.h"

#include <fstream>
#include <string>
#include "interpreter/Lexer.h"
#include "interpreter/Parser.h"
#include "server/JsonEncoder.h"
#include "unistd.h"
#include "Logger.h"

int main(int argc, char *argv[])
{
    int PORT = 9999;
    std::cout << "Enter port number: ";
    std::cin >> PORT;
    std::cout << "\n";

    ServerManager *manager = ServerManager::getInstance(PORT);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
