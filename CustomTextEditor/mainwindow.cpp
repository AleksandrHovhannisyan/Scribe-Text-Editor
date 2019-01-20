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
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tabbedEditor = ui->tabWidget;
    tabbedEditor->setTabsClosable(true);

    initializeStatusBarLabels();
    on_currentTab_changed(0);

    toggleUndo(false);
    toggleRedo(false);
    toggleCopyAndCut(false);

    connect(tabbedEditor, SIGNAL(currentChanged(int)), this, SLOT(on_currentTab_changed(int)));
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(on_actionSave_or_actionSaveAs_triggered()));
    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(on_actionSave_or_actionSaveAs_triggered()));
    connect(ui->actionReplace, SIGNAL(triggered()), this, SLOT(on_actionFind_triggered()));
}


/* Performs all necessary memory cleanup operations on dynamically allocated objects.
 */
MainWindow::~MainWindow()
{
    delete wordLabel;
    delete wordCountLabel;
    delete charLabel;
    delete charCountLabel;
    delete columnCountLabel;
    delete columnLabel;
    delete ui;
}


/* Called each time the current tab changes in the tabbed editor. Sets the main window's current editor,
 * reconnects any relevant signals, and updates the window.
 */
void MainWindow::on_currentTab_changed(int index)
{
    editor = qobject_cast<Editor*>(tabbedEditor->widget(index));
    connect(editor, SIGNAL(windowNeedsToBeUpdated(DocumentMetrics)), this, SLOT(updateWindow(DocumentMetrics)));
    connect(editor, SIGNAL(undoAvailable(bool)), this, SLOT(toggleUndo(bool)));
    connect(editor, SIGNAL(redoAvailable(bool)), this, SLOT(toggleRedo(bool)));
    connect(editor, SIGNAL(copyAvailable(bool)), this, SLOT(toggleCopyAndCut(bool)));
    updateWindow(editor->getDocumentMetrics());
}


/* Initializes and updates the status bar labels.
 */
void MainWindow::initializeStatusBarLabels()
{
    wordLabel = new QLabel("Words: ");
    wordCountLabel = new QLabel();
    charLabel = new QLabel("Chars: ");
    charCountLabel = new QLabel();
    columnLabel = new QLabel("Column: ");
    columnCountLabel = new QLabel();
    ui->statusBar->addPermanentWidget(wordLabel);
    ui->statusBar->addPermanentWidget(wordCountLabel);
    ui->statusBar->addPermanentWidget(charLabel);
    ui->statusBar->addPermanentWidget(charCountLabel);
    ui->statusBar->addPermanentWidget(columnLabel);
    ui->statusBar->addPermanentWidget(columnCountLabel);
}


/* Updates the window and status bar labels to reflect the most up-to-date document metrics.
 */
void MainWindow::updateWindow(DocumentMetrics metrics)
{
    QString wordText = QString::number(metrics.wordCount) + tr("   ");
    QString charText = QString::number(metrics.charCount) + tr("   ");
    QString colText = QString::number(metrics.currentColumn) + tr("   ");

    wordCountLabel->setText(wordText);
    charCountLabel->setText(charText);
    columnCountLabel->setText(colText);

    QString fileName = editor->getFileName();
    bool editorUnsaved = editor->isUnsaved();

    tabbedEditor->setTabText(tabbedEditor->currentIndex(), fileName + (editorUnsaved ? "*" : ""));
    setWindowTitle(fileName + (editorUnsaved ? "[Unsaved]" : ""));
}


/* Launches a dialog box asking the user if they would like to save the current file.
 * If the user selects "No" or closes the dialog window, the file will not be saved.
 * Otherwise, if they select "Yes," the file will be saved.
 */
QMessageBox::StandardButton MainWindow::allowUserToSave()
{
    QString fileName = editor->getFileName();

    QMessageBox::StandardButton userSelection;
    userSelection = Utility::promptYesOrNo(this, "Unsaved changes", tr("Do you want to save the changes to ") +
                                  fileName + tr("?"));

    if(userSelection == QMessageBox::Yes)
    {
        on_actionSave_or_actionSaveAs_triggered();
    }

    return userSelection;
}


/* Called when the user selects the New option from the menu or toolbar (or uses Ctrl+N).
 * Adds a new tab to the editor.
 */
void MainWindow::on_actionNew_triggered() { tabbedEditor->add(new Editor()); }


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

    ui->statusBar->showMessage("Document saved", 2000);

    // Save the contents of the editor to the disk and close the file descriptor
    QTextStream out(&file);
    QString editorContents = editor->toPlainText();
    out << editorContents;
    file.close();

    QString fileName = editor->getFileName();
    editor->setFileNeedsToBeSaved(false);
    tabbedEditor->setTabText(tabbedEditor->currentIndex(), fileName);
    setWindowTitle(fileName);
}


/* Called when the user selects the Open option from the menu or toolbar
 * (or uses Ctrl+O). If the current document has unsaved changes, it first
 * asks the user if they want to save. In any case, it launches a dialog box
 * that allows the user to select the file they want to open. Sets the editor's
 * current file path to that of the opened file on success and updates the app state.
 */
void MainWindow::on_actionOpen_triggered()
{
    bool openInCurrentTab = editor->isUntitled() && !editor->isUnsaved();

    // Ask the user to specify the name of the file
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open"));

    // Don't do anything if the user hit Cancel
    if(filePath.isNull())
    {
        return;
    }

    // Attempt to create a file descriptor for the file at the given path
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }
    ui->statusBar->showMessage("Loaded file", 2000);

    // Read the file contents into the editor and close the file descriptor
    QTextStream in(&file);
    QString documentContents = in.readAll();

    if(!openInCurrentTab)
    {
        tabbedEditor->add(new Editor());
    }
    editor->setCurrentFilePath(filePath);
    editor->setPlainText(documentContents);
    file.close();

    // Changing the editor's text above will trigger editor->on_textChanged, which will set
    // fileNeedsToBeSaved to true, but we need to reset it here because we don't need to save
    editor->setFileNeedsToBeSaved(false);
    tabbedEditor->setTabText(tabbedEditor->currentIndex(), editor->getFileName());
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
        ui->statusBar->showMessage("Printing", 2000);
    }
}


// TODO connect the shortcut Ctrl+W to tab closing, not application exiting
/* Called when the user selects the Exit option from the menu (or uses Ctrl+W).
 * Allows the  * user to save any unsaved files before quitting.
 */
void MainWindow::on_actionExit_triggered()
{
    for(int i = 0; i < tabbedEditor->count(); i++)
    {
        Editor * tab = qobject_cast<Editor*>(tabbedEditor->widget(i));

        if(tab->isUnsaved())
        {
            Editor *current = editor;
            editor = tab;
            allowUserToSave();
            editor = current;
        }
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


/* Called when the user explicitly selects the Go To option from the menu (or uses Ctrl+G).
 * Launches a Go To dialog that prompts the user to enter a line number they wish to jump to.
 */
void MainWindow::on_actionGo_To_triggered() { editor->launchGotoDialog(); }


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


/* Called when the user selects the Font option from the menu. Launches a font selection dialog.
 */
void MainWindow::on_actionFont_triggered() { editor->launchFontDialog(); }


/* Called when the user selects the Auto Indent option from the Format menu.
 */
void MainWindow::on_actionAuto_Indent_triggered() { editor->toggleAutoIndent(ui->actionAuto_Indent->isChecked()); }


/* Toggles the visibility of the status bar.
 */
void MainWindow::on_actionStatus_Bar_triggered() { ui->statusBar->setVisible(!ui->statusBar->isVisible()); }


/* Overrides the QWidget closeEvent virtual method. Called when the user tries
 * to close the main application window conventually via the red X. Allows the
 * user to save any unsaved files before quitting.
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    on_actionExit_triggered();
}
