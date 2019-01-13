#include "mainwindow.h"
#include "utilityfunctions.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QtPrintSupport/QPrinter>      // printing
#include <QtPrintSupport/QPrintDialog>  // printing
#include <QFileDialog>                  // file open/save dialogs
#include <QFile>                        // file descriptors, IO
#include <QTextStream>                  // file IO
#include <QDateTime>                    // current time
#include <QApplication>                 // quit


/* Sets up the main application window and all of its children/widgets.
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    editor = ui->textEdit;
    editor->setFont("Courier", QFont::Monospace, true, 10, 5);

    connect(editor, SIGNAL(windowNeedsToBeUpdated(DocumentMetrics)), this, SLOT(updateWindow(DocumentMetrics)));

    initializeStatusBarLabels(); // must do this before editor->reset() to ensure labels are initialized
    editor->reset();
    toggleRedo(false);
    toggleCopyAndCut(false);

    ui->actionUndo->setEnabled(false);

    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(on_actionSave_or_actionSaveAs_triggered()));
    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(on_actionSave_or_actionSaveAs_triggered()));
    connect(ui->actionReplace, SIGNAL(triggered()), this, SLOT(on_actionFind_triggered()));

    connect(editor, SIGNAL(undoAvailable(bool)), this, SLOT(toggleUndo(bool)));
    connect(editor, SIGNAL(redoAvailable(bool)), this, SLOT(toggleRedo(bool)));
    connect(editor, SIGNAL(copyAvailable(bool)), this, SLOT(toggleCopyAndCut(bool)));
}


/* Performs all necessary memory cleanup operations on dynamically allocated objects.
 */
MainWindow::~MainWindow()
{
    delete wordCountLabel;
    delete charCountLabel;
    delete columnLabel;
    delete ui;
}


/* Initializes and updates the status bar labels.
 */
void MainWindow::initializeStatusBarLabels()
{
    wordCountLabel = new QLabel();
    charCountLabel = new QLabel();
    columnLabel = new QLabel();
    ui->statusBar->addWidget(wordCountLabel);
    ui->statusBar->addWidget(charCountLabel);
    ui->statusBar->addWidget(columnLabel);
}


/* Updates the window and status bar labels to reflect the most up-to-date document metrics.
 */
void MainWindow::updateWindow(DocumentMetrics metrics)
{
    QString wordText = tr("   Words: ") + QString::number(metrics.wordCount) + tr("   ");
    QString charText = tr("   Chars: ") + QString::number(metrics.charCount) + tr("   ");
    QString colText = tr("   Column: ") + QString::number(metrics.currentColumn);
    wordCountLabel->setText(wordText);
    charCountLabel->setText(charText);
    columnLabel->setText(colText);

    QString fileName = editor->getFileName();
    if(fileName.isEmpty()) { fileName = defaultWindowTitle; }
    setWindowTitle(fileName.append(editor->isUnsaved() ? tr(" [Unsaved]") : ""));
}


/* Launches a dialog box asking the user if they would like to save the current file.
 * If the user selects "No" or closes the dialog window, the file will not be saved.
 * Otherwise, if they select "Yes," the file will be saved.
 */
void MainWindow::allowUserToSave()
{
    QString fileName = editor->getFileName();
    if(fileName.isEmpty()) { fileName = defaultWindowTitle; }

    QMessageBox::StandardButton userSelection;
    userSelection = Utility::promptYesOrNo(this, "Unsaved changes", tr("Do you want to save the changes to ") +
                                  fileName + tr("?"));

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
    if(editor->isUnsaved())
    {
        allowUserToSave();
    }
    editor->reset();
}


/* Called when the user selects the Save or Save As option from the menu or toolbar
 * (or uses Ctrl+S). On success, saves the contents of the text editor to the disk using
 * the file name provided by the user. If the current document was never saved, or if the
 * user chose Save As, the program prompts the user to specify a name and directory for the file.
 */
void MainWindow::on_actionSave_or_actionSaveAs_triggered()
{
    bool saveAs = sender() == ui->actionSave_As;
    QString currentFilePath = editor->getCurrentFilePath();

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
        editor->setCurrentFilePath(filePath);
    }

    // Attempt to create a file descriptor with the given path
    QFile file(editor->getCurrentFilePath());
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

    editor->setFileNeedsToBeSaved(false);
    setWindowTitle(editor->getFileName());
}


/* Called when the user selects the Open option from the menu or toolbar
 * (or uses Ctrl+O). If the current document has unsaved changes, it first
 * asks the user if they want to save. In any case, it launches a dialog box
 * that allows the user to select the file they want to open. Sets the editor's
 * current file path to that of the opened file on success and updates the app state.
 */
void MainWindow::on_actionOpen_triggered()
{
    // Ensure we save any unsaved contents before opening a new file
    if(editor->isUnsaved())
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
    editor->setCurrentFilePath(filePath);

    // Attempt to create a file descriptor for the file at the given path
    QFile file(filePath);
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

    // Changing the editor's text above will trigger editor->on_textChanged, which will set
    // fileNeedsToBeSaved to true, but we need to reset it here because we don't need to save
    editor->setFileNeedsToBeSaved(false);
    setWindowTitle(editor->getFileName());
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
    if(editor->isUnsaved())
    {
        allowUserToSave();
    }

    QApplication::quit();
}


/* Called when the Undo operation is toggled by the editor.
 */
void MainWindow::toggleUndo(bool undoAvailable) { ui->actionUndo->setEnabled(undoAvailable); }


/* Called when the Redo operation is toggled by the editor.
 */
void MainWindow::toggleRedo(bool redoAvailable) { ui->actionRedo->setEnabled(redoAvailable); }


/* Called when the user performs the Undo operation.
 */
void MainWindow::on_actionUndo_triggered() { if(ui->actionUndo->isEnabled()) editor->undo(); }


/* Called when the user performs the Redo operation.
 */
void MainWindow::on_actionRedo_triggered(){ if(ui->actionRedo->isEnabled()) editor->redo(); }


/* Called when the Copy and Cut operations are toggled by the editor.
 */
void MainWindow::toggleCopyAndCut(bool copyCutAvailable)
{
    ui->actionCopy->setEnabled(copyCutAvailable);
    ui->actionCut->setEnabled(copyCutAvailable);
}


/* Called when the user performs the Cut operation.
 */
void MainWindow::on_actionCut_triggered() { if(ui->actionCut->isEnabled()) editor->cut(); }


/* Called when the user performs the Copy operation.
 */
void MainWindow::on_actionCopy_triggered() { if(ui->actionCopy->isEnabled()) editor->copy(); }


/* Called when the user performs the Paste operation.
 */
void MainWindow::on_actionPaste_triggered() { editor->paste(); }


/* Called when the user explicitly selects the Find option from the menu
 * (or uses Ctrl+F). Launches a dialog that prompts the user to enter a search query.
 */
void MainWindow::on_actionFind_triggered() { editor->launchFindDialog(); }


void MainWindow::on_actionGo_To_triggered()
{
    // TODO fill in code
}


/* Called when the user explicitly selects the Select All option from the menu (or uses Ctrl+A).
 */
void MainWindow::on_actionSelect_All_triggered() { editor->selectAll(); }


/* Called when the user explicitly selects the Time/Date option from the menu (or uses F5).
 */
void MainWindow::on_actionTime_Date_triggered()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    editor->insertPlainText(currentTime.toString());
}


/* Toggles the visibility of the status bar labels.
 */
void MainWindow::on_actionStatus_Bar_triggered()
{
    wordCountLabel->setVisible(!wordCountLabel->isVisible());
    charCountLabel->setVisible(!charCountLabel->isVisible());
    columnLabel->setVisible(!columnLabel->isVisible());
}


/* Overrides the QWidget closeEvent virtual method. Called when the user tries
 * to close the main application window. If the current document is unsaved,
 * it allows the user to save before finally exiting.
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    // If unsaved document in progress, pause termination and allow user to save
    if(editor->isUnsaved())
    {
        event->ignore();
        allowUserToSave();
    }

    // Finally allow the exit to go through
    event->accept();
}
