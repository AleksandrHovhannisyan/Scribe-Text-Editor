#include "editor.h"
#include "linenumberarea.h"
#include <QPainter>
#include <QTextBlock>


/* Initializes this Editor.
 */
Editor::Editor(QWidget *parent) : QPlainTextEdit (parent)
{
    lineNumberArea = new LineNumberArea(this);
    findDialog = new FindDialog();
    findDialog->setParent(this, Qt::Tool | Qt::MSWindowsFixedSizeDialogHint);

    connect(this, SIGNAL(blockCountChanged()), this, SLOT(updateLineNumberAreaWidth()));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));

    reset();
    updateLineNumberAreaWidth();
    highlightCurrentLine();
}


/* Performs all necessary memory cleanup operations.
 */
Editor::~Editor()
{
    delete lineNumberArea;
}


/* Resets the editor to its default state.
 */
void Editor::reset()
{
    currentFilePath.clear();
    fileNeedsToBeSaved = false;
    metrics = DocumentMetrics();
}


/* Private helper function that extracts a file name from the current file path stored internally.
 */
QString Editor::getFileNameFromPath() const
{
    if(currentFilePath.isEmpty())
    {
        return currentFilePath;
    }

    int indexOfLastForwardSlash = currentFilePath.lastIndexOf('/');
    int lengthOfFileName = currentFilePath.length() - indexOfLastForwardSlash;

    QString fileName = currentFilePath.mid(indexOfLastForwardSlash + 1, lengthOfFileName);
    return fileName;
}


/* Returns the name of the file corresponding to the contents currently in the editor.
 */
QString Editor::getFileName() const
{
    return getFileNameFromPath();
}


/* Sets the editor's font using the specified parameters.
 * @param family - the name of the font family
 * @param styleHint - used to select an appropriate default font family if the specified one is unavailable.
 * @param fixedPitch - if true, monospace font (equal-width characters)
 * @param pointSize - the size, in points, of the desired font (e.g., 12 for 12-pt font)
 * @param tabStopWidth - the desired width of a tab in terms of the equivalent number of spaces
 */
void Editor::setFont(QString family, QFont::StyleHint styleHint, bool fixedPitch, int pointSize, int tabStopWidth)
{
    font.setFamily(family);
    font.setStyleHint(styleHint);
    font.setFixedPitch(fixedPitch);
    font.setPointSize(pointSize);
    QPlainTextEdit::setFont(font);

    QFontMetrics metrics(font);
    setTabStopWidth(tabStopWidth * metrics.width(' '));
}


/* Launches the Find dialog box if it isn't already visible and sets its focus.
 */
void Editor::launchFindDialog()
{
    if(findDialog->isHidden())
    {
        findDialog->show();
        findDialog->activateWindow();
        findDialog->raise();
        findDialog->setFocus();
    }
}





/* -----------------------------------------------------------
 * All functions below this line are used for lineNumberArea
 * -----------------------------------------------------------
 */




/* Returns the width of the editor's line number area by computing the number of digits in the last
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
