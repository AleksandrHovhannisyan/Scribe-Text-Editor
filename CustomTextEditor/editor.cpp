#include "editor.h"
#include "linenumberarea.h"
#include "utilityfunctions.h"
#include <QPainter>
#include <QTextBlock>
#include <QtDebug>

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
    connect(this, SIGNAL(textChanged()), this, SLOT(on_textChanged()));

    connect(findDialog, SIGNAL(queryTextReady(QString, bool, bool, bool)), this, SLOT(on_findQueryText_ready(QString, bool, bool, bool)));
    connect(findDialog, SIGNAL(replacementTextReady(QString)), this, SLOT(on_replacementText_ready(QString)));

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
    setPlainText(QString()); // this will trigger on_textChanged
    fileNeedsToBeSaved = false;
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


/* Called when the findDialog object emits its queryTextReady signal. Initiates the
 * actual searching within the editor. If a match is found, it's highlighted in the editor.
 * Otherwise, feedback is given to the user in the search window, which remains open for further
 * searches.
 * @param queryText - the text the user wants to search for
 * @param findNext - flag denoting whether the search should find the next instance of the query
 * @param caseSensitive - flag denoting whether the search should heed the case of results
 * @param wholeWords - flag denoting whether the search should look for whole word matches or partials
 */
void Editor::on_findQueryText_ready(QString queryText, bool findNext, bool caseSensitive, bool wholeWords)
{
    // Keep track of cursor position prior to search
    int cursorPositionPriorToSearch = textCursor().position();

    // Specify the options we'll be searching with
    QTextDocument::FindFlags searchOptions = QTextDocument::FindFlags();    
    if(caseSensitive)
    {
        searchOptions |= QTextDocument::FindCaseSensitively;
    }
    if(wholeWords)
    {
        searchOptions |= QTextDocument::FindWholeWords;
    }

    // Search until the end of the document
    bool matchFound = find(queryText, searchOptions);

    // If we didn't find a match, search from top of document
    if(!matchFound)
    {
        moveCursor(QTextCursor::Start);
        matchFound = find(queryText, searchOptions);

        // If we land back at the start position, don't count it as a match
        if(textCursor().position() == cursorPositionPriorToSearch)
        {
            QMessageBox::information(this, "Title", "Looped back to start pos.");

            // Reset the cursor to its original position prior to searching
            QTextCursor newCursor = textCursor();
            newCursor.setPosition(cursorPositionPriorToSearch);
            setTextCursor(newCursor);
        }
    }

    if(!matchFound)
    {
        // Reset the cursor to its original position prior to searching
        QTextCursor newCursor = textCursor();
        newCursor.setPosition(cursorPositionPriorToSearch);
        setTextCursor(newCursor);

        // Inform the user of the unsuccessful search
        QMessageBox::information(findDialog, tr("Find"), tr("No results found."));

        // If findDialog triggered searching from a replace all
        findDialog->concludeReplaceAll(); // set some sort of boolean flag to false that FindDialog checks in while loop of replace all!
    }
}


/* Called when the user clicks the Replace button in FindDialog, which emits a signal to pass
 * along the replacement text.
 * @param replacementText - the string to be used for replacement
 */
void Editor::on_replacementText_ready(QString replacementText)
{
    // TODO this still inserts even if no match was found, fix
    QTextCursor cursor = textCursor();
    cursor.beginEditBlock();
    cursor.insertText(replacementText);
    cursor.endEditBlock();
}


/* Scans the entire document character by character and tallies the number of
 * characters, words, and lines and storing the counts internally for reporting.
 */
void Editor::updateFileMetrics()
{
    QString documentContents = toPlainText();
    int documentLength = documentContents.length();
    metrics = DocumentMetrics();
    QString currentWord = "";

    // Loop through each character in the document
    for(int i = 0; i < documentLength; i++)
    {
        char currentCharacter = documentContents[i].toLatin1();

        // Debug assertion error caused for invalid file formats like PDF
        if(currentCharacter < -1 || currentCharacter > 255)
        {
            return;
        }

        // Newline
        if(currentCharacter == '\n')
        {
            // Special case: newline following a word
            if(!currentWord.isEmpty())
            {
                metrics.wordCount++;
                currentWord.clear();
            }
        }
        // All other valid characters
        else
        {
            metrics.charCount++;

            // Alphanumeric character
            if(isalnum(currentCharacter))
            {
                currentWord += currentCharacter;
            }
            // Whitespace (excluding newline, handled separately above)
            else if(isspace(currentCharacter))
            {
                // Whitespace following a word means we completed a word
                if(!currentWord.isEmpty())
                {
                    metrics.wordCount++;
                    currentWord.clear();
                }
                // Consume all other instances of whitespace
                else
                {
                    while(i + 1 < documentLength && isspace(documentContents[i + 1].toLatin1()))
                    {
                        i++;
                    }
                }
            }
        }
    }

    // e.g., if we stopped typing and still had a word in progress, we need to count it
    if(!currentWord.isEmpty())
    {
        metrics.wordCount++;
        currentWord.clear();
    }

    // Column metric
    metrics.currentColumn = textCursor().positionInBlock() + 1;
}


/* Sets a flag denoting whether the current file has to be saved.
 */
void Editor::setFileNeedsToBeSaved(bool status) { fileNeedsToBeSaved = status; }


/* Called whenever the contents of the text editor change, even if they are deleted
 * and restored to their original state. Marks the document as needing to be saved
 * and updates the file metrics. Emits the windowNeedsToBeUpdated signal when it's done
 * to direct its parent window to update any information it displays to the user.
 */
void Editor::on_textChanged()
{
    fileNeedsToBeSaved = true;
    updateFileMetrics();
    emit(windowNeedsToBeUpdated(metrics));
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


/* Called when the cursor changes position. Highlights the line the cursor is on.
 */
void Editor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
       QTextEdit::ExtraSelection selection;
       QColor lineColor = QColor(Qt::lightGray).lighter(125);

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
