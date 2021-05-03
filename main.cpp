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
#include "Logger.h"
#include "QFile"
#include "QTextStream"
#include "QFileDialog"
#include "qmessagebox.h"


using namespace std;

int main(int argc, char *argv[])
{
    ServerManager *manager = ServerManager::getInstance(9999);

    Logger::EnableFileOutput();
    Logger::Info("EL logger sirve");
    Logger::Error("prueba de error");


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
