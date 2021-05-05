#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "string"
#include "interpreter/Parser.h"

using namespace std;

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

// foward declaration
class LogThread;

//! Class that handles the GUI and the events
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateRamView();
    void add_row(string dir, string label, string value, string count);
    void delete_row();
    void set_stdout_text(string text);
    void append_stdout_line(string text);
    void delete_text(int identifier);
    void hightligthLine(int fistLine);
    void deleteHightlight();

    //! Flag so the next line button knows whether a program is being run atm or not
    bool isRunning;

    //! Parser being utilized to parse through code
    Parser parser;

    //! The string being held in Stdout, it's an attribute so that the Mainwindow can pass itself down to the interpreter and it can modify this string
    string StdoutString;

private slots:
    void set_log_text(string text);
    void on_actionRun_triggered();

    void on_actionNext_line_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionCopy_triggered();

    void on_actionCut_triggered();

    void on_actionPaste_triggered();

    void on_actionAboutQt_triggered();

    void on_actionDelete_triggered();

    void on_pushButton_clicked();

    void on_actionStop_triggered();

private:
    Ui::MainWindow *ui;

    LogThread *mLogThread;

    int firstLine;

    //! emun to facilitate the column location with the label of the column
    enum Column
    {
        DIR,
        LABEL,
        VALUE,
        COUNT
    };
};
#endif // MAINWINDOW_H
