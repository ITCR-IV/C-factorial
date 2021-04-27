#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "string"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

//! Class that handles the GUI and the events
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void add_row(string dir, string label, string value, string count);
    void delete_row();

private slots:

    void on_actionRun_triggered();

    void on_actionNext_line_triggered();

    void on_actionPrev_line_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionPaste_triggered();

    void on_actionAboutQt_triggered();

    void on_actionDelete_triggered();

private:
    Ui::MainWindow *ui;

    enum Column
    {
        DIR, LABEL, VALUE, COUNT
    };
};
#endif // MAINWINDOW_H
