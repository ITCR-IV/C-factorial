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


/*!
 * \brief Construct a new MainWindow object
 *
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    ui->plainTextEdit->paste();
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
}

/*!
 * \brief Run the first line of code
 *
 */
void MainWindow::on_actionRun_triggered()
{
    //Start with a fresh memory state
    ServerManager *manager = ServerManager::getInstance(9999);
    manager->sendRequest(FLUSH);

    // Extract code and format it
    QString code = (ui->plainTextEdit->toPlainText());
    std::string plainCode = code.toUtf8().constData();
    string fullCode = "{\n" + plainCode + "\n}\n";

    // Create lexer and parser
    Lexer lexer = Lexer(fullCode);
    this->parser = Parser(lexer);
    this->parser.advance_1loc(); // Get through the first scope
    return;
}

/*!
 * \brief
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
            //Update Ram Live View with current state
            updateRamView();
        }
        catch (Lexer::SyntaxException syntaxE)
        {
            printf("Syntaxis error:\n");
            std::cout << syntaxE.what();
            this->isRunning = false;
        }
        catch (Parser::SemanticException semanticE)
        {
            printf("Parser error:\n");
            std::cout << semanticE.what();
            this->isRunning = false;
        }
        catch (Interpreter::RuntimeException runtimeE)
        {
            printf("Interpreter error:\n");
            std::cout << runtimeE.what();
            this->isRunning = false;
        }
        if (parser.scopeLevel == 0)
        {
            this->isRunning = false;
        }
    }
    return;
}

/*!
 * \brief
 *
 */
void MainWindow::on_actionPrev_line_triggered()
{
    this->set_log_text();
}

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
 * \brief sets the text on log textBrowser
 *
 */

void MainWindow::set_log_text()
{
    FILE * logFile;

    long fileSize;

    char * fileText;

    logFile = fopen("log.txt", "r");
    fseek (logFile , 0 , SEEK_END);

    fileSize = ftell (logFile);

    rewind (logFile);

    fileText = (char*) malloc (sizeof(char)*fileSize);

    fread(fileText, fileSize+1, 1, logFile);

    fclose(logFile);



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
    } else if (identifier == 2)
    {
        ui->textBrowser_2->clear();
    }
}
