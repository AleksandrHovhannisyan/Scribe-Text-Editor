#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ctype.h>              // isspace, isalnum
#include <QtDebug>

// TODO use textEdit->copyAvailable() to check if copy/cut can be performed; if not, disable copy/cut buttons
// TODO ensure we prompt a save before the user exits the application if there's still stuff there!

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    metrics(),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resetEditor();
    setFont("Courier", QFont::Monospace, true, 10);
    setTabStopWidth(5);

    // Have to manually connect these signals to the single slot to handle both
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(on_actionSave_or_actionSaveAs_triggered()));
    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(on_actionSave_or_actionSaveAs_triggered()));
}


MainWindow::~MainWindow()
{
    delete ui;
}


/* Resets the text editor to all of its defaults and
 * effectively creates an empty document from scratch.
 * See the constructor and on_actionNew_triggered().
 */
void MainWindow::resetEditor()
{
    currentFilePath.clear();
    ui->textEdit->setText(QString());
    setWindowTitle(defaultWindowTitle);
    fileNeedsToBeSaved = false;
}


/* Sets the editor's tab stop width to the specified value.
 * @param width - the width of the tab stop, in terms of the number of equivalent space characters.
 */
void MainWindow::setTabStopWidth(int width)
{
    tabStopWidth = width;
    QFontMetrics metrics(font);
    ui->textEdit->setTabStopWidth(tabStopWidth * metrics.width(' '));
}


/* Sets the editor's font using the specified parameters.
 * @param family - the name of the font family
 * @param styleHint - used to select an appropriate default font family if the specified one is unavailable.
 * @param fixedPitch - if true, monospace font (equal-width characters)
 * @param pointSize - the size, in points, of the desired font (e.g., 12 for 12-pt font)
 */
void MainWindow::setFont(QString family, QFont::StyleHint styleHint,
                       bool fixedPitch, int pointSize)
{
    font.setFamily(family);
    font.setStyleHint(styleHint);
    font.setFixedPitch(fixedPitch);
    font.setPointSize(pointSize);
    ui->textEdit->setFont(font);
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


/* TODO document
 */
void MainWindow::allowUserToSave()
{
    QMessageBox::StandardButton userSelection;

    userSelection = QMessageBox::question(this, "",
                          "Do you want to save the changes to " + getFileNameFromPath(currentFilePath) + "?",
                          QMessageBox::Yes | QMessageBox::No);

    if(userSelection == QMessageBox::Yes)
    {
        on_actionSave_or_actionSaveAs_triggered();
    }
}


/* Called when the user selects the New option from the menu or toolbar (or uses Ctrl+N).
 * If the current document has unsaved changes, it prompts the user to save or discard.
 * In either case, it ends up clearing the current file name and editor contents.
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
        // Try to get a valid file path
        QString filePath = QFileDialog::getSaveFileName(this, "Save");

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

    setWindowTitle(getFileNameFromPath(currentFilePath));
    QTextStream out(&file);

    QString editorContents = ui->textEdit->toPlainText();
    out << editorContents;

    file.close();
    fileNeedsToBeSaved = false;
}


/* Called when the user selects the Open option from the menu or toolbar
 * (or uses Ctrl+O). Launches a dialog box that allows the user to select
 * the file they want to open. Sets the editor's current file path to that
 * of the opened file on success.
 */
void MainWindow::on_actionOpen_triggered()
{
    // Ensure we save any unsaved contents before opening a new file
    if(fileNeedsToBeSaved)
    {
        allowUserToSave();
    }

    // Ask the user to specify the name of the file
    QString filePath = QFileDialog::getOpenFileName(this, "Open");

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

    setWindowTitle(getFileNameFromPath(currentFilePath));

    QTextStream in(&file);
    QString documentContents = in.readAll();

    ui->textEdit->setText(documentContents);
    file.close();
    fileNeedsToBeSaved = false;
}


/* Called when the user selects the Print option from the menu or toolbar (or uses Ctrl+P).
 * Allows the user to print the contents of the current document.
 */
void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName("Document printer");
    QPrintDialog printDialog(&printer, this);

    if(printDialog.exec() != QPrintDialog::Rejected)
    {
        ui->textEdit->print(&printer);
    }
}


/* Called when the user selects the Exit option from the menu (or uses Ctrl+W).
 * Ensures the user can safely exit without losing unsaved changes.
 * Terminates the program.
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
void MainWindow::on_actionUndo_triggered() { ui->textEdit->undo(); }


/* Called when the user performs the Redo operation.
 */
void MainWindow::on_actionRedo_triggered(){ ui->textEdit->redo(); }


/* Called when the user performs the Cut operation.
 */
void MainWindow::on_actionCut_triggered() { ui->textEdit->cut(); }


/* Called when the user performs the Copy operation.
 */
void MainWindow::on_actionCopy_triggered() { ui->textEdit->copy(); }


/* Called when the user performs the Paste operation.
 */
void MainWindow::on_actionPaste_triggered() { ui->textEdit->paste(); }


/* Called when the user explicitly selects the Find option from the menu.
 * Launches a dialog that prompts the user to enter a search query. Locates
 * the first matching instance in the document.
 */
void MainWindow::on_actionFind_triggered()
{
    // TODO add UI for searching
    ui->textEdit->find("");
}


void MainWindow::on_actionFind_Next_triggered()
{
    // TODO fill in code
}


void MainWindow::on_actionReplace_triggered()
{
    // TODO add UI for replacing
}


void MainWindow::on_actionGo_To_triggered()
{
    // TODO fill in code
}


/* Called when the user explicitly selects the Select All option from the menu.
 */
void MainWindow::on_actionSelect_All_triggered() { ui->textEdit->selectAll(); }


/* TODO document
 * TODO maybe think about optimizing the code here so we don't recalculate everything
 */
void MainWindow::updateFileMetrics()
{
    QString documentContents = ui->textEdit->toPlainText();
    int documentLength = documentContents.length();
    metrics = DocumentMetrics();
    QString currentWord = "";

    // Loop through each character in the document
    for(int i = 0; i < documentLength; i++)
    {
        char currentCharacter = documentContents[i].toLatin1();

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

    qDebug() << "Chars: " << metrics.charCount << " Words: " << metrics.wordCount << " Lines: " << metrics.lineCount;
}


/* Called whenever the contents of the text editor change, even if they are deleted
 * and restored to their original state. Sets a flag that the current file needs to
 * be saved before any file creation, opening, or exiting operations.
 */
void MainWindow::on_textEdit_textChanged()
{
    fileNeedsToBeSaved = true;
    updateFileMetrics();
    // TODO Send those metrics to the status bar
}


/* Overrides the QWidget closeEvent virtual method. Called when the user tries
 * to close the main application window. If the current document is unsaved,
 * it allows the user to save before finally exiting. Otherwise, if there are
 * no unsaved changes, it closes.
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    if(fileNeedsToBeSaved)
    {
        event->ignore();
        allowUserToSave();
    }
    event->accept();
}
