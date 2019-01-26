#include "editor.h"
#include "linenumberarea.h"
#include "utilityfunctions.h"
#include <QPainter>
#include <QTextBlock>
#include <QFontDialog>
#include <QTextDocumentFragment>
#include <QPalette>
#include <QStack>
#include <QSet>
#include <QQueue>
#include <QtDebug>


/* Initializes this Editor.
 */
Editor::Editor(QWidget *parent) : QPlainTextEdit (parent)
{
    document()->setModified(false);
    setLineWrapMode(QPlainTextEdit::LineWrapMode::NoWrap);

    metrics = DocumentMetrics();
    lineNumberArea = new LineNumberArea(this);
    defaultCharFormat.setUnderlineStyle(QTextCharFormat::NoUnderline);

    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth()));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(on_cursorPositionChanged()));
    connect(this, SIGNAL(textChanged()), this, SLOT(on_textChanged()));
    connect(this, SIGNAL(undoAvailable(bool)), this, SLOT(setUndoAvailable(bool)));
    connect(this, SIGNAL(redoAvailable(bool)), this, SLOT(setRedoAvailable(bool)));

    installEventFilter(this);
    updateLineNumberAreaWidth();
    on_cursorPositionChanged();
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
    metricCalculationEnabled = true;
    currentFilePath.clear();
    document()->setModified(false);
    setPlainText(QString());
}


/* Sets this Editor's current file path.
 * @newPath - the file path associated with the file this Editor represents
 */
void Editor::setCurrentFilePath(QString newPath)
{
    currentFilePath = newPath;
    fileIsUntitled = false;
}


/* Private helper function that extracts a file name from the current file path stored internally.
 */
QString Editor::getFileNameFromPath()
{
    if(currentFilePath.isEmpty())
    {
        fileIsUntitled = true;
        return "Untitled document";
    }

    int indexOfLastForwardSlash = currentFilePath.lastIndexOf('/');
    int lengthOfFileName = currentFilePath.length() - indexOfLastForwardSlash;

    QString fileName = currentFilePath.mid(indexOfLastForwardSlash + 1, lengthOfFileName);
    return fileName;
}


/* Launches a QFontDialog to allow the user to select a font.
 */
void Editor::launchFontDialog()
{
    bool userChoseFont;
    QFont font = QFontDialog::getFont(&userChoseFont, QFont("Courier", 10), this);

    if(userChoseFont)
    {
        setFont(font.family(), QFont::Monospace, true, font.pointSize(), 5);
    }
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
    int cursorPositionBeforeSearch = textCursor().position();

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
        bool previouslyFound = searchHistory.previouslyFound(query);

        // Log the first position at which this query was found in the current document state
        // Search history is always reset whenever we do a full cycle back to the first match or start a new search "chain"
        if(!previouslyFound)
        {
            searchHistory.add(query, cursorPositionBeforeSearch, foundPosition);
        }
        // If term was previously found, check that we didn't cycle back to the first-ever find
        else
        {
            bool loopedBackToFirstMatch = foundPosition == searchHistory.firstFoundAt(query);

            if(loopedBackToFirstMatch)
            {
                // It's not really a match that we found; it's a repeat of the very first ever match
                matchFound = false;

                // Reset the cursor to its original position prior to first search for this term
                QTextCursor newCursor = textCursor();
                int cursorPositionBeforeFirstSearch = searchHistory.cursorPositionBeforeFirstSearchFor(query);
                newCursor.setPosition(cursorPositionBeforeFirstSearch);
                setTextCursor(newCursor);

                // Clear search history
                searchHistory.clear();

                // Inform the user of the unsuccessful search
                emit(findResultReady("No more results found."));
            }
        }
    }
    else
    {
        // Reset the cursor to its position prior to this particular search
        QTextCursor newCursor = textCursor();
        newCursor.setPosition(cursorPositionBeforeSearch);
        setTextCursor(newCursor);

        // Inform the user of the unsuccessful search
        emit(findResultReady("No results found."));
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
    metricCalculationEnabled = false;

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
        emit(findResultReady("No results found."));
    }
    else
    {
        emit(findResultReady("Document searched. Replaced " + QString::number(replacements) + " instances."));
    }

    metricCalculationEnabled = true; // reset here
    updateFileMetrics();
    emit(windowNeedsToBeUpdated(metrics));
}


/* Called when the user clicks the Go button in the GotoDialog.
 */
void Editor::goTo(int line)
{
    if(line > blockCount() || line < 1)
    {
        emit(gotoResultReady("Invalid line number."));
        return;
    }

    QTextCursor cursor(document()->findBlockByLineNumber(line - 1));
    setTextCursor(cursor);
}


/* Applies the given formatting to the selection of text between the two given indices (inclusive).
 * Unformats all text before applying the given formatting if the flag is specified as true.
 * @param startIndex - the index from which the formatting should proceed
 * @param endIndex - the index at which the formatting should stop
 * @param format - the format to apply to the text between startIndex and endIndex, inclusive
 * @param unformatAllFirst - flag denoting whether all text should be unformatted before formatting (false by default)
 */
void Editor::formatSubtext(int startIndex, int endIndex, QTextCharFormat format, bool unformatAllFirst)
{
    QTextCursor cursorBeforeFormatting = textCursor();
    QTextCursor formatter = textCursor();

    if(unformatAllFirst)
    {
        QTextCursor unformatter = textCursor();
        unformatter.setPosition(0, QTextCursor::MoveAnchor);
        unformatter.setPosition(document()->toPlainText().length(), QTextCursor::KeepAnchor);
        unformatter.setCharFormat(defaultCharFormat);
        qDebug() << "Unformatting!";
        setTextCursor(unformatter);
    }

    formatter.setPosition(startIndex, QTextCursor::MoveAnchor);
    formatter.setPosition(endIndex, QTextCursor::KeepAnchor);
    formatter.setCharFormat(format);

    setTextCursor(formatter);
    setTextCursor(cursorBeforeFormatting);
}


/* Scans the entire document character by character and tallies the number of
 * characters and words and storing the counts internally for reporting.
 * Note: column counting is handled by highlightCurrentLine.
 */
void Editor::updateFileMetrics()
{
    QString documentContents = toPlainText().toUtf8();
    int documentLength = documentContents.length();
    metrics = DocumentMetrics();
    QString currentWord = "";

    // Loop through each character in the document
    for(int i = 0; i < documentLength; i++)
    {
        // Convert to unsigned char to avoid running into debug assertion problems
        unsigned char currentCharacter = (unsigned char)documentContents[i].toLatin1();

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
                currentWord += (char)currentCharacter;
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
                    while(i + 1 < documentLength && isspace((unsigned char)documentContents[i + 1].toLatin1()))
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
}


/* Called whenever the contents of the text editor change, even if they are deleted
 * and restored to their original state. Marks the document as needing to be saved
 * and updates the file metrics. Emits the windowNeedsToBeUpdated signal when it's done
 * to direct its parent window to update any information it displays to the user.
 */
void Editor::on_textChanged()
{
    searchHistory.clear();
    updateFileMetrics();
    emit(windowNeedsToBeUpdated(metrics));
}


/* Returns the indentation level of the current line of text.
 */
int Editor::indentationLevelOfCurrentLine()
{
    QTextCursor originalCursor = textCursor();
    moveCursorToStartOfCurrentLine();

    QString documentContents = document()->toPlainText();
    int indentationLevel = 0;

    int index = textCursor().position();
    if(index >= documentContents.length())
    {
        index--;
    }

    while(index < documentContents.length())
    {
        if(documentContents.at(index) == '\t')
        {
            indentationLevel++;
            index++;
        }
        else
        {
            break;
        }
    }

    setTextCursor(originalCursor);
    return indentationLevel;
}


/* Inserts the specified number of tabs in the document.
 */
void Editor::insertTabs(int numTabs)
{
    for(int i = 0; i < numTabs; i++)
    {
        insertPlainText("\t");
    }
}


/* Moves the cursor to the start of the current line.
 */
void Editor::moveCursorToStartOfCurrentLine()
{
    QTextCursor cursor = textCursor();

    do
    {
        cursor.movePosition(QTextCursor::Left);
        setTextCursor(cursor);
    }
    while(metrics.currentColumn != 1);
}


/* Custom handler for events. Used to handle the case of Enter being pressed after an opening brace
 * or the tab key being used on a selection of text.
 */
bool Editor::eventFilter(QObject* obj, QEvent* event)
{
    bool isKeyPress = event->type() == QEvent::KeyPress;

    if(isKeyPress)
    {
        int key = static_cast<QKeyEvent*>(event)->key();

        // Auto-indenting after ENTER
        if(key == Qt::Key_Enter || key == Qt::Key_Return)
        {
            QString documentContents = document()->toPlainText();

            if(documentContents.length() >= 1)
            {
                int indexToLeftOfCursor = textCursor().position() - 1;

                if(indexToLeftOfCursor >= 0 && indexToLeftOfCursor < documentContents.length())
                {
                    bool hitEnterAfterOpeningBrace = documentContents.at(indexToLeftOfCursor) == '{';

                    if(hitEnterAfterOpeningBrace)
                    {
                        bool alreadyPaired = Utility::braceIsBalanced(documentContents, indexToLeftOfCursor);

                        int braceLevel = indentationLevelOfCurrentLine();
                        insertPlainText("\n");
                        insertTabs(braceLevel + 1);

                        if(!alreadyPaired)
                        {
                            insertPlainText("\n");
                            insertTabs(braceLevel);
                            insertPlainText("}");

                            // Set the cursor so it's right after the nested tab
                            QTextCursor cursor = textCursor();
                            cursor.setPosition(cursor.position() - 2 - braceLevel);
                            setTextCursor(cursor);
                        }

                        return true;
                    }

                    // Make sure any other block of text gets aligned properly
                    else if(autoIndentEnabled)
                    {
                        int indentationLevel = indentationLevelOfCurrentLine();
                        insertPlainText("\n");
                        insertTabs(indentationLevel);
                        return true;
                    }
                }
            }
        }
        // Indenting selections of text
        else if(key == Qt::Key_Tab)
        {
            if(textCursor().hasSelection())
            {
                QString text = textCursor().selection().toPlainText();

                text.insert(0, '\t');
                for(int i = 1; i < text.length(); i++)
                {
                    // Insert a tab after each newline
                    if(text.at(i) == '\n' && i + 1 < text.length())
                    {
                        text.insert(i + 1, '\t');
                    }
                }

                // Replace the selection with the new tabbed text
                insertPlainText(text);
                return true;
            }
        }
        else
        {
            return QObject::eventFilter(obj, event);
        }
    }
    else
    {
        return QObject::eventFilter(obj, event);
    }

    return false;
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
void Editor::on_cursorPositionChanged()
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

    // When the cursor position changes, the column changes, so we need to update that
    if(metricCalculationEnabled)
    {
        metrics.currentColumn = textCursor().positionInBlock() + 1;
        emit(columnCountChanged(metrics.currentColumn));
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
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(), Qt::AlignRight, lineNumber);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}

