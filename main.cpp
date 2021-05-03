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

using namespace std;

int main(int argc, char *argv[])
{
    Logger::EnableFileOutput();
    Logger::Info("EL logger sirve");
    Logger::Error("prueba de error");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

    // //Server testing

    // ServerManager *man = ServerManager::getInstance();

    // string req;

    // //cin >> req;

    // //int ireq = stoi(req);

    // //int ireq = 3;
    // man->sendRequest(ireq);
    // UpdateInfo var = UpdateInfo("int", "x", "55");
    // JsonEncoder encoder = JsonEncoder(var);

    // man->sendJson(encoder.encode());
    // man->listenServer();

    //Parser testing

    // std::ifstream ifs("test.txt");
    // std::string plainCode((std::istreambuf_iterator<char>(ifs)),
    //                       (std::istreambuf_iterator<char>()));

    // //cout << plainCode;
    // //string plainCode = "int x = getAddr(x);";
    // //try
    // //{
    // /*std::cout << '"' << plainCode << '"' << '\n';
    // string fullCode = "{ " + plainCode + "\n}";
    // std::cout << "\n"
    //           << &fullCode << "\n";
    // Lexer lexer = Lexer(fullCode);
    // Parser parser = Parser(lexer);
    // parser.scope();*/
    // //}
    // /*catch (Lexer::SyntaxException e)
    // {
    //     cout << e.what();
    // }
    // catch (Parser::SemanticException e2)
    // {
    //     cout << e2.what();
    // }*/
}
