#include "Mainwindow.h"
#include "ui_Mainwindow.h"
#include "qmessagebox.h"
#include "QFileDialog"
#include "QFile"
#include "QTextStream"
#include "interpreter/Parser.h"
#include <string>
#include <iostream>

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
    QString code = (ui->plainTextEdit->toPlainText());
    std::string plainCode = code.toUtf8().constData();
    //std::cout << plainCode + '\n';
    Lexer lexer = Lexer("{" + plainCode + "\n}");
    Parser parser = Parser(lexer);

    parser.scope();
    //Hacer lexer y parser
    return;
}

/*!
 * \brief
 *
 */
void MainWindow::on_actionNext_line_triggered()
{
    //Correr método de parser para avanzar una línea
}

/*!
 * \brief
 *
 */
void MainWindow::on_actionPrev_line_triggered()
{
}
