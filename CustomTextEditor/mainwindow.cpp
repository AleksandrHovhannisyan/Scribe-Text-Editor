#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ctype.h>                      // isspace, isalnum
#include <QtDebug>
#include <QtPrintSupport/QPrinter>      // printing
#include <QtPrintSupport/QPrintDialog>  // printing
#include <QFileDialog>                  // file open/save dialogs
#include <QFile>                        // file descriptors, IO
#include <QTextStream>                  // file IO
#include <QFontMetrics>                 // tab stop width
#include <QApplication>                 // quit


/* Sets up the text editor with all necessary parameters. Resets the editor to its
 * default state, initializes status bar labels, and sets the default editor font.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    metrics(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    editor = ui->textEdit;

    initializeStatusBarLabels(); // must do this before resetEditor to ensure labels are initialized
    resetEditor();
    setFont("Courier", QFont::Monospace, true, 10, 5);

    findDialog = new FindDialog();
    findDialog->setParent(this, Qt::Tool | Qt::MSWindowsFixedSizeDialogHint);

    // Have to manually connect these signals to the same slot. Feature unavailable in designer.
    connect(ui->actionSave, SIGNAL(triggered()),
            this, SLOT(on_actionSave_or_actionSaveAs_triggered()));
    connect(ui->actionSave_As, SIGNAL(triggered()),
            this, SLOT(on_actionSave_or_actionSaveAs_triggered()));

    // The FindDialog object will emit queryTextReady when the query is ready for processing
    connect(findDialog, SIGNAL(queryTextReady(QString, bool, bool, bool)),
            this, SLOT(on_findQueryText_ready(QString, bool, bool, bool)));
}


/* Performs all necessary memory cleanup operations on dynamically allocated objects.
 */
MainWindow::~MainWindow()
{
    delete wordCountLabel;
    delete charCountLabel;
    delete lineCountLabel;
    delete findDialog;
    delete ui;
}

// TODO move this to Editor

/* Resets the text editor to all of its defaults and
 * effectively creates an empty document from scratch.
 * See the constructor and on_actionNew_triggered for uses.
 */
void MainWindow::resetEditor()
{
    currentFilePath.clear();
    editor->setPlainText("");
    setWindowTitle(defaultWindowTitle);
    fileNeedsToBeSaved = false;
    positionOfLastFindMatch = -1;
}


/* Initializes and updates the status bar labels.
 */
void MainWindow::initializeStatusBarLabels()
{
    wordCountLabel = new QLabel();
    charCountLabel = new QLabel();
    lineCountLabel = new QLabel(); // TODO get rid of this, no longer needed
    ui->statusBar->addWidget(wordCountLabel);
    ui->statusBar->addWidget(charCountLabel);
    ui->statusBar->addWidget(lineCountLabel);
    updateStatusBar();
}


/* Updates the status bar labels to reflect the most up-to-date document metrics.
 */
void MainWindow::updateStatusBar()
{
    QString wordText = tr("   Words: ") + QString::number(metrics.wordCount) + tr("   ");
    QString charText = tr("   Chars: ") + QString::number(metrics.charCount) + tr("   ");
    QString lineText = tr("   Lines: ") + QString::number(metrics.lineCount) + tr("   ");
    wordCountLabel->setText(wordText);
    charCountLabel->setText(charText);
    lineCountLabel->setText(lineText);
}


// TODO move to editor

/* Sets the editor's font using the specified parameters.
 * @param family - the name of the font family
 * @param styleHint - used to select an appropriate default font family if the specified one is unavailable.
 * @param fixedPitch - if true, monospace font (equal-width characters)
 * @param pointSize - the size, in points, of the desired font (e.g., 12 for 12-pt font)
 * @param tabStopWidth - the desired width of a tab in terms of the equivalent number of spaces
 */
void MainWindow::setFont(QString family, QFont::StyleHint styleHint,
                       bool fixedPitch, int pointSize, int tabStopWidth)
{
    font.setFamily(family);
    font.setStyleHint(styleHint);
    font.setFixedPitch(fixedPitch);
    font.setPointSize(pointSize);
    editor->setFont(font);

    QFontMetrics metrics(font);
    editor->setTabStopWidth(tabStopWidth * metrics.width(' '));
}


/* Launches a Yes or No message box that prompts the user to make a selection.
 * See allowUserToSave and on_findQueryText_ready for uses.
 */
QMessageBox::StandardButton MainWindow::promptYesOrNo(QString title, QString prompt)
{
    return QMessageBox::question(this, title, prompt, QMessageBox::Yes | QMessageBox::No);
}


/* Returns the actual name of the file that's part of the given path.
 * @param filePath - the forward-slash-delimited path of the file
 */
QString MainWindow::getFileNameFromPath(QString filePath)
{
    // For unsaved documents
    if(filePath.isEmpty())
    {
        return defaultWindowTitle;
    }
    int indexOfLastForwardSlash = filePath.lastIndexOf('/');
    QString fileName = filePath.mid(indexOfLastForwardSlash + 1, filePath.length() - indexOfLastForwardSlash);
    return fileName;
}


/* Launches a dialog box asking the user if they would like to save the current file.
 * If the user selects "No" or closes the dialog window, the file will not be saved.
 * Otherwise, if they select "Yes," the file will be saved.
 */
void MainWindow::allowUserToSave()
{
    QMessageBox::StandardButton userSelection;

    userSelection = promptYesOrNo("Unsaved changes", tr("Do you want to save the changes to ") +
                                  getFileNameFromPath(currentFilePath) + tr("?"));

    if(userSelection == QMessageBox::Yes)
    {
        on_actionSave_or_actionSaveAs_triggered();
    }
}


/* Called when the user selects the New option from the menu or toolbar (or uses Ctrl+N).
 * If the current document has unsaved changes, it prompts the user to save or discard.
 * In either case, it ends up resetting the editor/document.
 */
void MainWindow::on_actionNew_triggered()
{
    // Don't create a new empty doc if there are unsaved changes in the current one
    if(fileNeedsToBeSaved)
    {
        allowUserToSave();
    }

    resetEditor();
}


/* Called when the user selects the Save or Save As option from the menu or toolbar
 * (or uses Ctrl+S). On success, saves the contents of the text editor to the disk using
 * the file name provided by the user. If the current document was never saved, or if the
 * user chose Save As, the program prompts the user to specify a name and directory for the file.
 */
void MainWindow::on_actionSave_or_actionSaveAs_triggered()
{
    bool saveAs = sender() == ui->actionSave_As;

    // If user hit Save As or user hit Save but current document was never saved to disk
    if(saveAs || currentFilePath.isEmpty())
    {
        // Title to be used for saving dialog
        QString saveDialogWindowTitle = saveAs ? tr("Save As") : tr("Save");

        // Try to get a valid file path
        QString filePath = QFileDialog::getSaveFileName(this, saveDialogWindowTitle);

        // Don't do anything if the user changes their mind and hits Cancel
        if(filePath.isNull())
        {
            return;
        }
        currentFilePath = filePath;
    }

    // Attempt to create a file descriptor with the given path
    QFile file(currentFilePath);
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    // Save the contents of the editor to the disk and close the file descriptor
    QTextStream out(&file);
    QString editorContents = editor->toPlainText();
    out << editorContents;
    file.close();

    fileNeedsToBeSaved = false;
    setWindowTitle(getFileNameFromPath(currentFilePath));
}


/* Called when the user selects the Open option from the menu or toolbar
 * (or uses Ctrl+O). If the current document has unsaved changes, it first
 * asks the user if they want to save. In any case, it launches a dialog box
 * that allows the user to select the file they want to open. Sets the editor's
 * current file path to thatof the opened file on success and updates the app state.
 */
void MainWindow::on_actionOpen_triggered()
{
    // Ensure we save any unsaved contents before opening a new file
    if(fileNeedsToBeSaved)
    {
        allowUserToSave();
    }

    // Ask the user to specify the name of the file
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open"));

    // Ensure we don't change the current file path if the user hit Cancel
    if(filePath.isNull())
    {
        return;
    }
    currentFilePath = filePath;

    // Attempt to create a file descriptor for the file at the given path
    QFile file(currentFilePath);
    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    // Read the file contents into the editor and close the file descriptor
    QTextStream in(&file);
    QString documentContents = in.readAll();
    editor->setPlainText(documentContents);
    file.close();

    // Changing the edit text above will trigger on_textEdit_changed, which will set
    // fileNeedsToBeSaved to true, but we need to reset it here because we don't need to save
    fileNeedsToBeSaved = false;
    setWindowTitle(getFileNameFromPath(currentFilePath));
}


/* Called when the user selects the Print option from the menu or toolbar (or uses Ctrl+P).
 * Allows the user to print the contents of the current document.
 */
void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName(tr("Document printer"));
    QPrintDialog printDialog(&printer, this);

    if(printDialog.exec() != QPrintDialog::Rejected)
    {
        editor->print(&printer);
    }
}


/* Called when the user selects the Exit option from the menu (or uses Ctrl+W).
 * Ensures that the user can safely exit without losing unsaved changes. Terminates the program.
 */
void MainWindow::on_actionExit_triggered()
{
    if(fileNeedsToBeSaved)
    {
        allowUserToSave();
    }

    QApplication::quit();
}


/* Called when the user performs the Undo operation.
 */
void MainWindow::on_actionUndo_triggered() { editor->undo(); }


/* Called when the user performs the Redo operation.
 */
void MainWindow::on_actionRedo_triggered(){ editor->redo(); }


/* Called when the user performs the Cut operation.
 */
void MainWindow::on_actionCut_triggered() { editor->cut(); }


/* Called when the user performs the Copy operation.
 */
void MainWindow::on_actionCopy_triggered() { editor->copy(); }


/* Called when the user performs the Paste operation.
 */
void MainWindow::on_actionPaste_triggered() { editor->paste(); }


/* Called when the user explicitly selects the Find option from the menu
 * (or uses Ctrl+F). Launches a dialog that prompts the user to enter a search query.
 */
void MainWindow::on_actionFind_triggered()
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
void MainWindow::on_findQueryText_ready(QString queryText, bool findNext, bool caseSensitive, bool wholeWords)
{
    // Keep track of cursor position prior to search
    int cursorPositionPriorToSearch = editor->textCursor().position();

    // If this is a repeat search, start from last found location; otherwise, start from current location
    if(findNext && positionOfLastFindMatch != -1)
    {
        editor->textCursor().setPosition(positionOfLastFindMatch);
    }

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

    // Don't worry about empty queryText, findDialog takes care of that on its end
    bool matchFound = editor->find(queryText, searchOptions);

    // If we didn't find a match, ask user if they want to search from top of document
    if(!matchFound)
    {
        QMessageBox::StandardButton userSelection;
        userSelection = promptYesOrNo("Find", "Reached end of document. Search from start?");

        if(userSelection == QMessageBox::StandardButton::Yes)
        {
            editor->moveCursor(QTextCursor::Start);
            matchFound = editor->find(queryText, searchOptions);
        }

        findDialog->activateWindow();
    }

    // TODO in addition to the position of the last find, keep track of the position of the first find. If we cycle back to start, then display no results found.

    // Final evaluation after second chance given
    if(matchFound)
    {
        positionOfLastFindMatch = editor->textCursor().position();
    }
    else
    {        
        // If we try another Find Next, this will indicate we never found one to begin with
        positionOfLastFindMatch = -1;

        // Reset the cursor to its original position prior to searching
        QTextCursor newCursor = editor->textCursor();
        newCursor.setPosition(cursorPositionPriorToSearch);
        editor->setTextCursor(newCursor);

        // Inform the user of the unsuccessful search
        QMessageBox::information(findDialog, tr("Find"), tr("No results found."));
    }
}


void MainWindow::on_actionReplace_triggered()
{
    // TODO add UI for replacing
}


void MainWindow::on_actionGo_To_triggered()
{
    // TODO fill in code
}


/* Called when the user explicitly selects the Select All option from the menu (or uses Ctrl+A).
 */
void MainWindow::on_actionSelect_All_triggered() { editor->selectAll(); }


/* Toggles the visibility of the status bar labels for word, char, and line count.
 */
void MainWindow::on_actionStatus_Bar_triggered()
{
    wordCountLabel->setVisible(!wordCountLabel->isVisible());
    charCountLabel->setVisible(!charCountLabel->isVisible());
    lineCountLabel->setVisible(!lineCountLabel->isVisible());
}


/* Scans the entire document character by character and tallies the number of
 * characters, words, and lines and storing the counts internally for reporting.
 */
void MainWindow::updateFileMetrics()
{
    QString documentContents = editor->toPlainText();
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
            metrics.lineCount++;
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

    // qDebug() << "Chars: " << metrics.charCount << " Words: " << metrics.wordCount << " Lines: " << metrics.lineCount;
}


/* Called whenever the contents of the text editor change, even if they are deleted
 * and restored to their original state. Updates the file metrics, window title, and status bar.
 */
void MainWindow::on_textEdit_textChanged()
{
    fileNeedsToBeSaved = true;
    QString newWindowTitle = getFileNameFromPath(currentFilePath).append(tr(" [Unsaved]"));
    setWindowTitle(newWindowTitle);
    updateFileMetrics();
    updateStatusBar();

    // We have to do this in case the user is sneaky and adds in another copy
    // of the previous search between two searches
    positionOfLastFindMatch = -1;
}


/* Overrides the QWidget closeEvent virtual method. Called when the user tries
 * to close the main application window. If the current document is unsaved,
 * it allows the user to save before finally exiting.
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    // If unsaved document in progress, pause termination and allow user to save
    if(fileNeedsToBeSaved)
    {
        event->ignore();
        allowUserToSave();
    }

    // Special case: Ctrl+F followed by a close = find dialog lingers, so forcefully close it
    if(!findDialog->isHidden())
    {
        findDialog->close();
    }

    // Finally allow the exit to go through
    event->accept();
}
