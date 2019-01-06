#include "editor.h"
#include "linenumberarea.h"
#include <QPainter>
#include <QTextBlock>

Editor::Editor(QWidget *parent) : QPlainTextEdit (parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged()), this, SLOT(updateLineNumberAreaWidth()));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth();
    highlightCurrentLine();
}


Editor::~Editor()
{
    delete lineNumberArea;
}


int Editor::getLineNumberAreaWidth()
{
    int lastLineNumber = qMax(1, blockCount());
    int numDigitsInLastLine = QString::number(lastLineNumber).length();

    int maxWidthOfAnyDigit = fontMetrics().width(QLatin1Char('9'));

    return numDigitsInLastLine * maxWidthOfAnyDigit + lineNumberAreaPadding;
}


void Editor::updateLineNumberAreaWidth()
{
    setViewportMargins(getLineNumberAreaWidth() + lineNumberAreaPadding, 0, 0, 0);
}


void Editor::updateLineNumberArea(const QRect &rectToBeRedrawn,
                                  int numPixelsScrolledVertically)
{
    if(numPixelsScrolledVertically != 0)
    {
        lineNumberArea->scroll(0, numPixelsScrolledVertically);
    }
    else
    {
        lineNumberArea->update(0, rectToBeRedrawn.y(),
                               lineNumberArea->width(), rectToBeRedrawn.height());
    }

    if(rectToBeRedrawn.contains(viewport()->rect()))
    {
        updateLineNumberAreaWidth();
    }
}


void Editor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(),
                                      getLineNumberAreaWidth(), cr.height()));
}


// TODO not working
void Editor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

       if (!isReadOnly())
       {
           QTextEdit::ExtraSelection selection;
           QColor lineColor = QColor(Qt::gray).lighter(160);

           selection.format.setBackground(lineColor);
           selection.format.setProperty(QTextFormat::FullWidthSelection, true);
           selection.cursor = textCursor();
           selection.cursor.clearSelection();
           extraSelections.append(selection);
       }

    setExtraSelections(extraSelections);
}


void Editor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
