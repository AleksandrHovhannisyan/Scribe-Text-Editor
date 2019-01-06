#ifndef EDITOR_H
#define EDITOR_H
#include <QPlainTextEdit>


/* Disclaimer: the code for painting the editor line numbers was not written by me.
 * I only changed some of the variable names and code to make things clearer,
 * but most of the content comes from this official Qt tutorial:
 *
 * http://doc.qt.io/qt-5/qtwidgets-widgets-codeeditor-example.html
 */


class Editor : public QPlainTextEdit
{
    Q_OBJECT

public:
    Editor(QWidget *parent = nullptr);
    ~Editor() override;
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int getLineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth();
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rectToBeRedrawn,
                              int numPixelsScrolledVertically);

private:
    QWidget *lineNumberArea;
    const int lineNumberAreaPadding = 30;

};

#endif // EDITOR_H