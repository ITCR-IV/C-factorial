#include "Mainwindow.h"
#include "ui_Mainwindow.h"
#include "qmessagebox.h"
#include "QFileDialog"
#include "QFile"
#include "QTextStream"
#include <string>
#include <iostream>
#include "server/RequestConstants.h"
#include "server/UpdateInfo.h"
#include "server/JsonDecoder.h"
#include "interpreter/Parser.h"
#include "ServerManager.h"
#include "Logger.h"
#include <thread>
#include "LogThread.h"

/*!
 * \brief Construct a new MainWindow object
 *
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), parser(Parser(Lexer(""), *this)), isRunning(false) //have to construct a parser with an empty lexer
{
    ui->setupUi(this);
    mLogThread = new LogThread(1, 1000, this);
    mLogThread->start();
    connect(mLogThread, &LogThread::logData, ui->textBrowser, &QTextBrowser::setText);
    firstLine = 1;
}

/*!
 * \brief deletes the mainWindow
 */
MainWindow::~MainWindow()
{

    delete ui;
}

/*!
 * \brief open a file into the IDE
 *
 */
void MainWindow::on_actionOpen_triggered()
{
    QFile file;
    QTextStream io;
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this, "Abrir");
    file.setFileName(fileName);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    if (!file.isOpen())
    {
        Logger::EnableFileOutput();
        Logger::Error(file.errorString().toStdString().c_str());
        QMessageBox::critical(this, "Error", file.errorString());
        return;
    }
    io.setDevice(&file);
    ui->plainTextEdit->setPlainText(io.readAll());
    file.flush();
    file.close();
}

/*!
 * \brief Saves the code into a txt
 *
 */
void MainWindow::on_actionSave_triggered()
{
    QFile file;
    QTextStream io;
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this, "Guardar");
    file.setFileName(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    if (!file.isOpen())
    {
        QMessageBox::critical(this, "Error", file.errorString());
        Logger::EnableFileOutput();
        Logger::Error(file.errorString().toStdString().c_str());
        return;
    }
    io.setDevice(&file);
    io << ui->plainTextEdit->toPlainText();
    file.close();
}

/*!
 * \brief Copy text
 *
 */
void MainWindow::on_actionCopy_triggered()
{
    ui->plainTextEdit->copy();
}

/*!
 * \brief cut text
 *
 */
void MainWindow::on_actionCut_triggered()
{
    ui->plainTextEdit->cut();
}

/*!
 * \brief Paste text
 *
 */
void MainWindow::on_actionPaste_triggered()
{
    //ui->plainTextEdit->paste();
    if (firstLine == 1){
       hightligthLine(1);
       //firstLine = 0;
    } else{
        hightligthLine(0);
    }

}

/*!
 * \brief shows information about Qt
 *
 */
void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this, "Qt");
}

/*!
 * \brief
 *
 */
void MainWindow::on_actionDelete_triggered()
{
    deleteHightligth();
}

/*!
 * \brief Stop the interpreter
 *
 */
void MainWindow::on_actionStop_triggered()
{

}

/*!
 * \brief Test code and run the first line of code
 *
 */
void MainWindow::on_actionRun_triggered()
{
    // Extract code and format it
    QString code = (ui->plainTextEdit->toPlainText());
    std::string plainCode = code.toUtf8().constData();

    if (plainCode == "")
    {
        return; //do nothing if no input is given
    }

    string fullCode = "{\n" + plainCode + "\n}\n";

    //Test with a fresh memory state
    ServerManager *manager = ServerManager::getInstance();
    manager->sendRequest(FLUSH);
    this->isRunning = false;
    this->StdoutString = "";
    this->set_stdout_text("");

    // Create lexer and parser for test run
    Lexer testLexer = Lexer(fullCode);
    Parser testParser = Parser(testLexer, *this);
    try
    {
        testParser.scope();
    }
    catch (Lexer::SyntaxException syntaxE)
    {
        printf("Syntax error:\n");
        std::cout << syntaxE.what();
        this->isRunning = true; // it needs to be running to be able to add anything ti Stdout
        this->append_stdout_line(syntaxE.what());
        this->isRunning = false;
        Logger::EnableFileOutput();
        Logger::Error(syntaxE.what());
        return;
    }
    catch (Parser::SemanticException semanticE)
    {
        printf("Parser error:\n");
        std::cout << semanticE.what();
        this->isRunning = true;
        this->append_stdout_line(semanticE.what());
        this->isRunning = false;
        Logger::EnableFileOutput();
        Logger::Error(semanticE.what());
        return;
    }
    catch (Interpreter::RuntimeException runtimeE)
    {
        printf("Interpreter error:\n");
        std::cout << runtimeE.what();
        this->isRunning = true;
        this->append_stdout_line(runtimeE.what());
        this->isRunning = false;
        Logger::EnableFileOutput();
        Logger::Error(runtimeE.what());
        return;
    }

    // Refresh memory state for real run
    manager->sendRequest(FLUSH);
    this->isRunning = true;

    // Create real lexer and parser and start
    Lexer lexer = Lexer(fullCode);
    this->parser = Parser(lexer, *this);
    this->parser.advance_1loc(); // Get through the first scope
    return;
}

/*!
 * \brief this method run the code line by line
 *
 */
void MainWindow::on_actionNext_line_triggered()
{
    //Correr método de parser para avanzar una línea
    if (isRunning)
    {
        try
        {
            parser.advance_1loc();
            if (firstLine == 1){
               hightligthLine(1);
               firstLine = 0;
            } else{
                hightligthLine(0);
            }

            //Update Ram Live View with current state
            updateRamView();
        }
        catch (Lexer::SyntaxException syntaxE)
        {
            printf("Syntax error:\n");
            std::cout << syntaxE.what();
            this->append_stdout_line(syntaxE.what());
            this->isRunning = false;
            Logger::EnableFileOutput();
            Logger::Error(syntaxE.what());
        }
        catch (Parser::SemanticException semanticE)
        {
            printf("Parser error:\n");
            std::cout << semanticE.what();
            this->append_stdout_line(semanticE.what());
            this->isRunning = false;
            Logger::EnableFileOutput();
            Logger::Error(semanticE.what());
        }
        catch (Interpreter::RuntimeException runtimeE)
        {
            printf("Interpreter error:\n");
            std::cout << runtimeE.what();
            this->append_stdout_line(runtimeE.what());
            this->isRunning = false;
            Logger::EnableFileOutput();
            Logger::Error(runtimeE.what());
        }
        if (parser.scopeLevel == 0)
        {
            this->isRunning = false;
            printf("PROGRAM FINISHED\n\n");
            Logger::EnableFileOutput();
            Logger::Info("Programa finalizado");
        }
    }
    else
    {
        printf("Program is not currently being ran, click the run button to start!");
        Logger::EnableFileOutput();
        Logger::Info("Program is not currently being ran, click the run button to start!");
    }
    return;
}


//! Whenever this is called the Ram view info is flushed completely, then current memory state is requested from the memory server and the ram view is updated line by line
void MainWindow::updateRamView()
{
    delete_row();
    //std::cout << "\n\nREQUESTING CURRENT MEMORY:\n";

    ServerManager *man = ServerManager::getInstance();
    man->sendRequest(REQUESTMEMORYSTATE);
    std::string info = man->getServerMsg(); // Get first packet of info
    while (info != "0")                     // Server will send a SUCCESS ("0") when it's done
    {
        // Do stuff with info
        // std::cout << "INFO: ";
        // std::cout << info << "\n";
        UpdateInfo infoObj = JsonDecoder(info).decode();
        add_row(to_string(infoObj.getDataAddress()), infoObj.getDataName(), infoObj.getDataValue(), to_string(infoObj.getDataCount()));

        // Get next info packet
        info = man->getServerMsg();
    }
}

/*!
 * \brief add a new row with string data
 *
 * \param dir string with the memory direction of the variable
 * \param label string with the label that identifies the variable
 * \param value string with the value of the variable
 * \param count string with the number of the pointers assigned to each memory location
 *
 */
void MainWindow::add_row(string dir, string label, string value, string count)
{
    int row = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
    ui->tableWidget->setItem(row, DIR, new QTableWidgetItem(QString::fromStdString(dir)));
    ui->tableWidget->setItem(row, LABEL, new QTableWidgetItem(QString::fromStdString(label)));
    ui->tableWidget->setItem(row, VALUE, new QTableWidgetItem(QString::fromStdString(value)));
    ui->tableWidget->setItem(row, COUNT, new QTableWidgetItem(QString::fromStdString(count)));
}

/*!
 * \brief deletes all the rows
 *
 */
void MainWindow::delete_row()
{
    ui->tableWidget->setRowCount(0);
}

/*!
* \brief sets the text on stdout textBrowser
*
*/
void MainWindow::set_stdout_text(string text)
{
    QString qstr = QString::fromStdString(text);
    ui->textBrowser_2->setText(qstr);
}

/*!
 * \brief Appends a new line to the stdout with the text given
 * 
 * \param text that line of text desired to be appended
 */
void MainWindow::append_stdout_line(string text)
{
    if (!this->isRunning)
    { // if not running then do nothing (this is so test run doesn't print anything)
        return;
    }
    if (this->StdoutString != "")
    {
        // Add newline to end of string
        this->StdoutString += '\n';
    }
    // Add text of new line
    this->StdoutString += text;
    // Set new stdout as new string
    this->set_stdout_text(this->StdoutString);
}

//! sets the text on log textBrowser
void MainWindow::set_log_text(string fileText)
{
    QString qstr = QString::fromStdString(fileText);
    ui->textBrowser->setText(qstr);
}

/*!
 * \brief deletes all the text on textBrowsers
 *
 */
void MainWindow::delete_text(int identifier)
{
    if (identifier == 1)
    {
        ui->textBrowser->clear();
    }
    else if (identifier == 2)
    {
        ui->textBrowser_2->clear();
    }
}

/*!
 * \brief deletes the text on log file
 *
 */
void MainWindow::on_pushButton_clicked()
{
    FILE *logFile;
    logFile = fopen("log.txt", "w");
    fclose(logFile);
}

void MainWindow::hightligthLine(int firstLine)
{
    if (firstLine==1){
        QTextCursor cur = ui->plainTextEdit->textCursor();
        QTextBlockFormat f;
        cur.setBlockFormat(f);
        f.setBackground(Qt::green);
        cur.movePosition(QTextCursor::Start);
        ui->plainTextEdit->setTextCursor(cur);
        cur.select(QTextCursor::LineUnderCursor);
        cur.setBlockFormat(f);
    } else {
        QTextCursor cur = ui->plainTextEdit->textCursor();
        QTextBlockFormat f;
        QTextBlockFormat P;
        P.setBackground(Qt::white);
        cur.setBlockFormat(P);
        f.setBackground(Qt::green);
        cur.movePosition(QTextCursor::NextBlock);
        ui->plainTextEdit->setTextCursor(cur);
        cur.select(QTextCursor::LineUnderCursor);
        cur.setBlockFormat(f);
    }

}

void MainWindow::deleteHightligth()
{
    QTextCursor cur = ui->plainTextEdit->textCursor();
    QTextBlockFormat P;
    P.setBackground(Qt::white);
    cur.movePosition(QTextCursor::Start);
    cur.setBlockFormat(P);
    bool tmp = true;
    int blockCount = ui->plainTextEdit->blockCount();
    while (tmp) {
        if (cur.position() == blockCount-1){
            cur.setBlockFormat(P);
            tmp = false;
        } else {
            cur.setBlockFormat(P);
            cur.movePosition(QTextCursor::NextBlock);
        }
    }
}
