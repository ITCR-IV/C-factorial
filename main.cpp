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

using namespace std;

int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);
    //MainWindow w;
    //w.show();
    //return a.exec();

    std::ifstream ifs("test.txt");
    std::string plainCode((std::istreambuf_iterator<char>(ifs)),
                          (std::istreambuf_iterator<char>()));

    //cout << plainCode;
    //string plainCode = "int x = getAddr(x);";
    //try
    //{
    std::cout << '"' << plainCode << '"' << '\n';
    string fullCode = "{ " + plainCode + "\n}";
    std::cout << "\n"
              << &fullCode << "\n";
    Lexer lexer = Lexer(fullCode);
    Parser parser = Parser(lexer);
    parser.scope();
    //}
    /*catch (Lexer::SyntaxException e)
    {
        cout << e.what();
    }
    catch (Parser::SemanticException e2)
    {
        cout << e2.what();
    }*/
}
