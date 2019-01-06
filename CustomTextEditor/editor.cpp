#include "editor.h"
#include "linenumberarea.h"
#include <QPainter>
#include <QTextBlock>


/* Initializes this Editor.
 */
Editor::Editor(QWidget *parent) : QPlainTextEdit (parent)
{
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(blockCountChanged()), this, SLOT(updateLineNumberAreaWidth()));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    updateLineNumberAreaWidth();
    highlightCurrentLine();
}


/* Performs all necessary memory cleanup operations.
 */
Editor::~Editor()
{
    delete lineNumberArea;
}


/* Returns the width of the line number area by computing the number of digits in the last
 * line number in the editor, multiplying this by the max width of any digit, and then adding
 * a fixed amount of padding.
 */
int Editor::getLineNumberAreaWidth()
{
    int lastLineNumber = blockCount();
    int numDigitsInLastLine = QString::number(lastLineNumber).length();

    int maxWidthOfAnyDigit = fontMetrics().width(QLatin1Char('9')); // 9 chosen here arbitrarily

    return numDigitsInLastLine * maxWidthOfAnyDigit + lineNumberAreaPadding;
}


/* Called when the user changes the number of blocks (paragraphs) in the document. Updates
 * the left margin of the editor's viewport so that its width is set to the widest line number.
 */
void Editor::updateLineNumberAreaWidth()
{
    setViewportMargins(getLineNumberAreaWidth() + lineNumberAreaPadding, 0, 0, 0);
}


/* Called when the editor viewport is scrolled. Redraws the line number area accordingly.
 */
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


/* Called when the editor is resized. Resizes the line number area accordingly.
 */
void Editor::resizeEvent(QResizeEvent *event)
{
    QPlainTextEdit::resizeEvent(event);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(),
                                      getLineNumberAreaWidth(), cr.height()));
}


// TODO not working

/* Called when the cursor changes position. Highlights the line the cursor is on.
 */
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


/* See linenumberarea.h for the call. Loops through each block (paragraph/line) in the
 * editor and paints the corresponding line numbers in the lineNumberArea.
 */
void Editor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    // Loop through each block (paragraph) and paint its corresponding number
    while (block.isValid() && top <= event->rect().bottom())
    {
        if (block.isVisible() && bottom >= event->rect().top())
        {
            QString lineNumber = QString::number(blockNumber + 1);
            painter.setPen(Qt::black);
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, lineNumber);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}
