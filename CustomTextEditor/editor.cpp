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

    connect(findDialog, SIGNAL(startFinding(QString, bool, bool)), this, SLOT(find(QString, bool, bool)));
    connect(findDialog, SIGNAL(startReplacing(QString, QString, bool, bool)), this, SLOT(replace(QString, QString, bool, bool)));
    connect(findDialog, SIGNAL(startReplacingAll(QString, QString, bool, bool)), this, SLOT(replaceAll(QString, QString, bool, bool)));

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
    metricCalculationDisabled = false;
    currentFilePath.clear();
    setPlainText(QString()); // this will trigger on_textChanged
    fileNeedsToBeSaved = false;
    findDialog->clearSearchHistory();
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


/* Returns a QTextDocument::FindFlags representing all the flags a search should be conducted with.
 * @param caseSensitive - flag denoting whether the search should heed the case of results
 * @param wholeWords - flag denoting whether the search should look for whole word matches or partials
 */
QTextDocument::FindFlags Editor::getSearchOptionsFromFlags(bool caseSensitive, bool wholeWords)
{
    QTextDocument::FindFlags searchOptions = QTextDocument::FindFlags();
    if(caseSensitive)
    {
        searchOptions |= QTextDocument::FindCaseSensitively;
    }
    if(wholeWords)
    {
        searchOptions |= QTextDocument::FindWholeWords;
    }
    return searchOptions;
}


/* Called when the findDialog object emits its queryReady signal. Initiates the
 * actual searching within the editor. First searches for a match from the current
 * position to the end of the document. If nothing is found, the search proceeds
 * from the beginning of the document, but stops once it encounters the first match
 * (if any) that was found in prior iterations for the current query.
 * @param query - the text the user wants to search for
 * @param caseSensitive - flag denoting whether the search should heed the case of results
 * @param wholeWords - flag denoting whether the search should look for whole word matches or partials
 */
bool Editor::find(QString query, bool caseSensitive, bool wholeWords)
{
    // Keep track of the cursor position prior to this search so we can return to it if no match is found
    int cursorPositionPriorToSearch = textCursor().position();

    // Specify the options we'll be searching with
    QTextDocument::FindFlags searchOptions = getSearchOptionsFromFlags(caseSensitive, wholeWords);

    // Search from the current position until the end of the document
    bool matchFound = QPlainTextEdit::find(query, searchOptions);

    // If we didn't find a match, search from the top of the document
    if(!matchFound)
    {
        moveCursor(QTextCursor::Start);
        matchFound = QPlainTextEdit::find(query, searchOptions);
    }

    // If we found a match, just make sure it's not a repeat
    if(matchFound)
    {
        int foundPosition = textCursor().position();
        bool previouslyFound = findDialog->previouslyFound(query);

        // Log the first position at which this query was found in the current document state
        // Search history is always reset whenever we do a full cycle back to the first match or start a new search "chain"
        if(!previouslyFound)
        {
            findDialog->addToSearchHistory(query, cursorPositionPriorToSearch, foundPosition);
        }
        // If term was previously found, check that we didn't cycle back to the first-ever find
        else
        {
            bool loopedBackToFirstMatch = foundPosition == findDialog->firstPositionOf(query);

            if(loopedBackToFirstMatch)
            {
                qDebug() << "---LOOPED BACK TO FIRST HIT---";

                // It's not really a match that we found; it's a repeat of the very first ever match
                matchFound = false;

                // Reset the cursor to its original position prior to first search for this term
                QTextCursor newCursor = textCursor();
                int positionPriorToFirstSearch = findDialog->positionPriorToFirstSearch(query);
                newCursor.setPosition(positionPriorToFirstSearch);
                setTextCursor(newCursor);

                // Clear search history
                findDialog->clearSearchHistory();

                // Inform the user of the unsuccessful search
                informUser("Find and Replace", "No more results found.");
            }
        }
    }
    else
    {
        // Reset the cursor to its position prior to this particular search
        QTextCursor newCursor = textCursor();
        newCursor.setPosition(cursorPositionPriorToSearch);
        setTextCursor(newCursor);

        // Inform the user of the unsuccessful search
        informUser("Find and Replace", "No results found.");
    }

    return matchFound;
}


/* Called when the user clicks the Replace button in FindDialog.
 * @param what - the string to find and replace
 * @param with - the string with which to replace any match
 * @param caseSensitive - flag denoting whether the search should heed the case of results
 * @param wholeWords - flag denoting whether the search should look for whole word matches or partials
 */
void Editor::replace(QString what, QString with, bool caseSensitive, bool wholeWords)
{
    metricCalculationDisabled = false;
    bool found = find(what, caseSensitive, wholeWords);

    if(found)
    {
        QTextCursor cursor = textCursor();
        cursor.beginEditBlock();
        cursor.insertText(with);
        cursor.endEditBlock();
    }
}


/* Called when the user clicks the Replace All button in FindDialog.
 * @param what - the string to find and replace
 * @param with - the string with which to replace all matches
 * @param caseSensitive - flag denoting whether the search should heed the case of results
 * @param wholeWords - flag denoting whether the search should look for whole word matches or partials
 */
void Editor::replaceAll(QString what, QString with, bool caseSensitive, bool wholeWords)
{
    // Optimization, don't update screen until the end of all replacements
    metricCalculationDisabled = true;

    // Search the entire document from the very beginning
    QTextCursor cursor = textCursor();
    cursor.setPosition(0);
    setTextCursor(cursor);

    // Conduct an initial search; don't rely on our custom find
    QTextDocument::FindFlags searchOptions = getSearchOptionsFromFlags(caseSensitive, wholeWords);
    bool found = QPlainTextEdit::find(what, searchOptions);
    int replacements = 0;

    // Keep replacing while there are matches left
    cursor.beginEditBlock();
    while(found)
    {
        QTextCursor currentPosition = textCursor();
        currentPosition.insertText(with);
        replacements++;
        found = QPlainTextEdit::find(what, searchOptions);
    }
    cursor.endEditBlock();

    // End-of-operation feedback
    if(replacements == 0)
    {
        informUser("Replace All", "No results found.");
    }
    else
    {
        informUser("Replace All", "Document searched. Replaced " + QString::number(replacements) + " instances.");
    }

    metricCalculationDisabled = false; // reset here
    updateFileMetrics();
    emit(windowNeedsToBeUpdated(metrics));
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

    // Column changes whenever we type
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
    findDialog->clearSearchHistory();

    // There are some cases when updating file metrics is not ideal, such as during replace all
    if(!metricCalculationDisabled)
    {
        updateFileMetrics();
        emit(windowNeedsToBeUpdated(metrics));
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
void Editor::updateLineNumberArea(const QRect &rectToBeRedrawn, int numPixelsScrolledVertically)
{
    if(numPixelsScrolledVertically != 0)
    {
        lineNumberArea->scroll(0, numPixelsScrolledVertically);
    }
    else
    {
        lineNumberArea->update(0, rectToBeRedrawn.y(), lineNumberArea->width(), rectToBeRedrawn.height());
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
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), getLineNumberAreaWidth(), cr.height()));
}


/* Called when the cursor changes position. Highlights the line the cursor is on.
 * Also computes the current column within that line.
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

    // highlightCurrentLine is called when the cursor changes, so we need to update this as well
    if(!metricCalculationDisabled)
    {
        // Column changes whenever we move the cursor, not only when we type
        metrics.currentColumn = textCursor().positionInBlock() + 1;
        emit(windowNeedsToBeUpdated(metrics));
    }
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
