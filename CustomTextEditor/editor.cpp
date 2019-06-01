#include "editor.h"
#include "linenumberarea.h"
#include "utilityfunctions.h"
#include <QPainter>
#include <QTextBlock>
#include <QFontDialog>
#include <QTextDocumentFragment>
#include <QPalette>
#include <QStack>
#include <QFileInfo>
#include <QtDebug>
#include <QSettings>


/* Initializes this Editor.
 */
Editor::Editor(QWidget *parent) : QPlainTextEdit (parent)
{
    readSettings();
    document()->setModified(false);

    setProgrammingLanguage(Language::None);
    metrics = DocumentMetrics();
    lineNumberArea = new LineNumberArea(this);
    setFont(QFont("Courier", DEFAULT_FONT_SIZE), QFont::Monospace, true, NUM_CHARS_FOR_TAB);

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
    delete syntaxHighlighter;
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

    QFileInfo fileInfo(currentFilePath);
    return fileInfo.fileName();
}


/* Sets the editor's font using the specified parameters.
 * @param newFont - the font to be set
 * @param styleHint - used to select an appropriate default font family if the specified one is unavailable.
 * @param fixedPitch - if true, monospace font (equal-width characters)
 * @param tabStopWidth - the desired width of a tab in terms of the equivalent number of spaces
 */
void Editor::setFont(QFont newFont, QFont::StyleHint styleHint, bool fixedPitch, int tabStopWidth)
{
    font = newFont;
    font.setStyleHint(styleHint);
    font.setFixedPitch(fixedPitch);
    QPlainTextEdit::setFont(font);

    QFontMetrics metrics(font);
    setTabStopWidth(tabStopWidth * metrics.width(' '));
}


/* Sets this Editor's programming language to the given language.
 * @param language - the language that this Editor should use for
 * syntax highlighting
 */
void Editor::setProgrammingLanguage(Language language)
{
    if(language == this->programmingLanguage)
    {
        return;
    }

    this->programmingLanguage = language;
    this->syntaxHighlighter = generateHighlighterFor(language);
}


/* Returns a Highlighter corresponding to the given language.
 * @param language - the programming language for which a
 * syntax highlighter should be generated
 */
Highlighter *Editor::generateHighlighterFor(Language language)
{
    QTextDocument *doc = document();

    switch (language)
    {
        case(Language::C): return cHighlighter(doc);
        case(Language::CPP): return cppHighlighter(doc);
        case(Language::Java): return javaHighlighter(doc);
        case(Language::Python): return pythonHighlighter(doc);
        default: return nullptr;
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
    int cursorPositionBeforeCurrentSearch = textCursor().position();

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

    // If we found a match...
    if(matchFound)
    {
        int foundPosition = textCursor().position();
        bool previouslyFound = searchHistory.previouslyFound(query);

        // If it's the first time finding this, log the first position at which this query was found in the current document state
        // Search history is always reset whenever we do a full cycle back to the first match or start a new search "chain"
        if(!previouslyFound)
        {
            searchHistory.add(query, cursorPositionBeforeCurrentSearch, foundPosition);
        }
        // If term was previously found, check that we didn't cycle back to the first-ever find
        else
        {
            bool loopedBackToFirstMatch = foundPosition == searchHistory.firstFoundAt(query);

            if(loopedBackToFirstMatch)
            {
                // It's not really a match that we found; it's a repeat of the very first-ever match
                matchFound = false;

                // Reset the cursor to its original position prior to first search for this term
                int cursorPositionBeforeFirstSearch = searchHistory.cursorPositionBeforeFirstSearchFor(query);
                moveCursorTo(cursorPositionBeforeFirstSearch);

                // Clear search history
                searchHistory.clear();

                // Inform the user of the unsuccessful search (note the "no MORE results found")
                emit(findResultReady("No more results found."));
            }
        }
    }
    else
    {
        // Reset the cursor to its position prior to this particular search
        moveCursorTo(cursorPositionBeforeCurrentSearch);

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
    moveCursorTo(0);

    // Conduct an initial search; don't rely on our custom find
    QTextDocument::FindFlags searchOptions = getSearchOptionsFromFlags(caseSensitive, wholeWords);
    bool found = QPlainTextEdit::find(what, searchOptions);
    int replacements = 0;

    // Keep replacing while there are matches left
    QTextCursor cursor(document());
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

    int beginningOfLine = document()->findBlockByLineNumber(line - 1).position();
    moveCursorTo(beginningOfLine);
}


// TODO obsolete unless we introduce error highlighting
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


/* Sets the Editor's line wrap mode to the given value.
 */
void Editor::setLineWrapMode(LineWrapMode lineWrapMode)
{
    QPlainTextEdit::setLineWrapMode(lineWrapMode);
    this->lineWrapMode = lineWrapMode;
}


/* Used to toggle the Editor's auto indentation mode (on/off).
 */
void Editor::toggleAutoIndent(bool autoIndent)
{
    autoIndentEnabled = autoIndent;

    // Update the setting in case any new tabs are opened later
    writeSetting(AUTO_INDENT_KEY, autoIndentEnabled);
}


/* Used to toggle the Editor's line wrap mode (wrapping or no wrapping).
 * @param wrap - flag denoting whether text should wrap (true) or not (false)
 */
void Editor::toggleWrapMode(bool wrap)
{
    if(wrap)
    {
        setLineWrapMode(LineWrapMode::WidgetWidth);
    }
    else
    {
        setLineWrapMode(LineWrapMode::NoWrap);
    }

    // Update the setting in case any new tabs are opened later
    writeSetting(LINE_WRAP_KEY, lineWrapMode);
}


/* Scans the entire document character by character and tallies the number of
 * characters and words and storing the counts internally for reporting.
 * Note: column counting is handled by highlightCurrentLine.
 */
void Editor::updateFileMetrics()
{
    QString documentContents = toPlainText().toUtf8();
    int documentLength = documentContents.length();

    DocumentMetrics oldMetrics = metrics;
    metrics = DocumentMetrics();
    metrics.currentColumn = oldMetrics.currentColumn;
    QString currentWord = "";

    // Loop through each character in the document
    for(int i = 0; i < documentLength; i++)
    {
        // Convert to unsigned char to avoid running into debug assertion problems
        unsigned char currentCharacter = qvariant_cast<unsigned char>(documentContents[i].toLatin1());

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
                currentWord += qvariant_cast<char>(currentCharacter);
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
                    while(i + 1 < documentLength &&
                          isspace(qvariant_cast<unsigned char>(documentContents[i + 1].toLatin1())))
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


/* Indents the selected text, if the cursor has a selection.
 * Returns true if it succeeds and false otherwise.
 */
void Editor::indentSelection(QTextDocumentFragment selection)
{
    QString text = selection.toPlainText();

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
}


/* Called when a user presses a key. Used to handle special formatting.
 */
bool Editor::handleKeyPress(QObject* obj, QEvent* event, int key)
{
    // Auto-indenting after ENTER
    if(key == Qt::Key_Enter || key == Qt::Key_Return)
    {
        QString documentContents = document()->toPlainText();
        int indexToLeftOfCursor = textCursor().position() - 1;

        if(documentContents.length() >= 1 &&
           indexToLeftOfCursor >= 0 &&
           indexToLeftOfCursor < documentContents.length())
        {
            QChar character = documentContents.at(indexToLeftOfCursor);

            // Hit ENTER after opening brace
            if(character == '{')
            {
                bool notPaired = Utility::closingBraceNeeded(documentContents);

                int braceLevel = indentationLevelOfCurrentLine();
                insertPlainText("\n");
                if(autoIndentEnabled) insertTabs(braceLevel + 1);

                if(notPaired)
                {
                    insertPlainText("\n");
                    insertTabs(braceLevel);
                    insertPlainText("}");

                    // Set the cursor so it's right after the nested tab
                    moveCursorTo(textCursor().position() - 2 - braceLevel);
                }

                return true;
            }
            // Hit ENTER after colon (for Python only)
            else if(character == ':' && programmingLanguage == Language::Python)
            {
                int level = indentationLevelOfCurrentLine();
                insertPlainText("\n");
                insertTabs(level + 1);
                return true;
            }
            // Hit ENTER after anything else
            else
            {
                if(autoIndentEnabled)
                {
                    int indentationLevel = indentationLevelOfCurrentLine();
                    insertPlainText("\n");
                    insertTabs(indentationLevel);
                }
                else
                {
                    insertPlainText("\n");
                }

                return true;
            }
        }
    }

    // Indenting selections of text
    else if(key == Qt::Key_Tab)
    {
        if(textCursor().hasSelection())
        {
            indentSelection(textCursor().selection());
            return true;
        }

        return false;
    }
    // Process anything else normally
    else
    {
        return QObject::eventFilter(obj, event);
    }
}


/* Moves this Editor's text cursor to the specified index position in the document.
 */
void Editor::moveCursorTo(int positionInText)
{
    QTextCursor newCursor = textCursor();
    newCursor.setPosition(positionInText);
    setTextCursor(newCursor);
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

        return handleKeyPress(obj, event, key);
    }
    else
    {
        return QObject::eventFilter(obj, event);
    }
}


/* Convenience function used for writing a single setting.
 * @param KEY - the string denoting the unique identifier for the setting
 * @param VAL - the type to be written and associated with KEY
 */
void Editor::writeSetting(const QString KEY, QVariant VAL) const
{
    QSettings settings;
    settings.setValue(KEY, VAL);
}


/* Preserves current settings for the editor so they can be
 * persisted into the next execution.
 */
void Editor::writeSettings()
{
    writeSetting(LINE_WRAP_KEY, lineWrapMode);
    writeSetting(AUTO_INDENT_KEY, autoIndentEnabled);
}


/* Loads previously saved settings for the editor.
 */
void Editor::readSettings()
{
    QSettings settings;

    if(settings.value(LINE_WRAP_KEY).isValid())
    {
        LineWrapMode previousLineWrapping = qvariant_cast<LineWrapMode>(settings.value(LINE_WRAP_KEY));
        setLineWrapMode(previousLineWrapping);
        lineWrapMode = previousLineWrapping;
    }

    if(settings.value(AUTO_INDENT_KEY).isValid())
    {
        bool previousAutoIndent = qvariant_cast<bool>(settings.value(AUTO_INDENT_KEY));
        autoIndentEnabled = previousAutoIndent;
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
    int top = qvariant_cast<int>(blockBoundingGeometry(block).translated(contentOffset()).top());
    int bottom = top + qvariant_cast<int>(blockBoundingRect(block).height());

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
        bottom = top + qvariant_cast<int>(blockBoundingRect(block).height());
        ++blockNumber;
    }
}

