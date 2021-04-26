#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>

//! Class that handles the behaviour of the code editor
class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

private:
    QWidget *lineNumberArea;

public:
    CodeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(const QRect &rect, int dy);

};

#endif // CODEEDITOR_H
