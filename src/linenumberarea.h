#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H
#include "editor.h"

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(Editor *editor) : QWidget(editor) { this->editor = editor; }
    QSize sizeHint() const override { return QSize(editor->getLineNumberAreaWidth(), 0); }

protected:
    void paintEvent(QPaintEvent *event) override { editor->lineNumberAreaPaintEvent(event); }

private:
    Editor *editor;
};

#endif // LINENUMBERAREA_H
