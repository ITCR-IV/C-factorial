#include "Mainwindow.h"
#include "ui_Mainwindow.h"
#include "qmessagebox.h"
#include "QFileDialog"
#include "QFile"
#include "QTextStream"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionOpen_triggered()
{
    QFile file;
    QTextStream io;
    QString fileName;
    fileName = QFileDialog::getOpenFileName(this,"Abrir");
    file.setFileName(fileName);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    if (!file.isOpen()){
        QMessageBox::critical(this,"Error", file.errorString());
        return;
    }
    io.setDevice(&file);
    ui->plainTextEdit->setPlainText(io.readAll());
    file.flush();
    file.close();
}

void MainWindow::on_actionSave_triggered()
{
    QFile file;
    QTextStream io;
    QString fileName;
    fileName = QFileDialog::getSaveFileName(this,"Guardar");
    file.setFileName(fileName);
    file.open(QIODevice::WriteOnly|QIODevice::Text);
    if (!file.isOpen()){
        QMessageBox::critical(this,"Error", file.errorString());
        return;
    }
    io.setDevice(&file);
    io<<ui->plainTextEdit->toPlainText();
    file.close();
}

void MainWindow::on_actionCopy_triggered()
{
    ui->plainTextEdit->copy();
}

void MainWindow::on_actionCut_triggered()
{
    ui->plainTextEdit->cut();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->plainTextEdit->paste();
}

void MainWindow::on_actionAboutQt_triggered()
{
    QMessageBox::aboutQt(this, "Qt");
}

void MainWindow::on_actionDelete_triggered()
{

}

void MainWindow::on_actionRun_triggered()
{

}

void MainWindow::on_actionNext_line_triggered()
{

}

void MainWindow::on_actionPrev_line_triggered()
{

}
