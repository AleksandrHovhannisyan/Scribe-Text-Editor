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
#include <QShortcut>


/* Sets up the main application window and all of its children/widgets.
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    installEventFilter(this);

    // Maps each menu language option to its corresponding Language type (for convenience)
    menuActionToLanguageMap[ui->actionC_Lang] = Language::C;
    menuActionToLanguageMap[ui->actionCPP] = Language::CPP;
    menuActionToLanguageMap[ui->actionJava_Lang] = Language::Java;
    menuActionToLanguageMap[ui->actionPython_Lang] = Language::Python;
    initializeLanguageMapping();

    // Used to ensure that only one language can ever be checked at a time
    languageGroup = new QActionGroup(this);
    languageGroup->setExclusive(true);
    languageGroup->addAction(ui->actionC_Lang);
    languageGroup->addAction(ui->actionCPP);
    languageGroup->addAction(ui->actionJava_Lang);
    languageGroup->addAction(ui->actionPython_Lang);
    connect(languageGroup, SIGNAL(triggered(QAction*)), this, SLOT(on_languageSelected(QAction*)));

    findDialog = new FindDialog();
    findDialog->setParent(this, Qt::Tool | Qt::MSWindowsFixedSizeDialogHint);

    gotoDialog = new GotoDialog();
    gotoDialog->setParent(this, Qt::Tool | Qt::MSWindowsFixedSizeDialogHint);

    tabbedEditor = ui->tabWidget;
    tabbedEditor->setTabsClosable(true);

    initializeStatusBarLabels();
    on_currentTab_changed(0);

    connect(tabbedEditor, SIGNAL(currentChanged(int)), this, SLOT(on_currentTab_changed(int)));
    connect(tabbedEditor, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(on_actionSave_or_actionSaveAs_triggered()));
    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(on_actionSave_or_actionSaveAs_triggered()));
    connect(ui->actionReplace, SIGNAL(triggered()), this, SLOT(on_actionFind_triggered()));

    QShortcut *tabCloseShortcut = new QShortcut(QKeySequence("Ctrl+W"), this);
    QObject::connect(tabCloseShortcut, SIGNAL(activated()), this, SLOT(closeTabShortcut()));

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
    delete languageGroup;
    delete ui;
}


/* Called when the user selects a language from the main menu. Sets the current language to
 * that language internally for the currently tabbed Editor.
 */
void MainWindow::on_languageSelected(QAction* language)
{
    editor->setProgrammingLanguage(menuActionToLanguageMap[language]);
    QStringList keywordPatterns = languageToKeywordMap[editor->getProgrammingLanguage()];
    syntaxHighlighter = new Highlighter(keywordPatterns, editor->document());
}


// TODO auto-identify languages using the file's extension once it's been saved
/* Fills its internal dictionary mapping languages to their corresponding keyword patterns.
 * Used by the main window to match different languages in the current document, depending
 * on the language the user has specified (or that the program has automatically identified).
 */
void MainWindow::initializeLanguageMapping()
{
    QStringList keywords;

    keywords << "\\bauto\\b" << "\\bbreak\\b" << "\\bcase\\b" << "\\bchar\\b" << "\\bconst\\b"
             << "\\bcontinue\\b" << "\\bdefault\\b" << "\\bdo\\b" << "\\bdouble\\b" << "\\belse\\b"
             << "\\benum\\b" << "\\bextern\\b" << "\\bfloat\\b" << "\\bfor\\b" << "\\bgoto\\b"
             << "\\bif\\b" << "\\bint\\b" << "\\blong\\b" << "\\bregister\\b" << "\\breturn\\b"
             << "\\bshort\\b" << "\\bsigned\\b" << "\\bsizeof\\b" << "\\bstatic\\b" << "\\bstruct\\b"
             << "\\bswitch\\b" << "\\btypedef\\b" << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvoid\\b"
             << "\\bvolatile\\b" << "\\bwhile\\b";

    languageToKeywordMap[Language::C] = keywords;

    keywords << "\\basm\\b" << "\\bnew\\b" << "\\bthis\\b" << "\\boperator\\b" << "\\bthrow\\b"
             << "\\bbool\\b" << "\\bexplicit\\b" << "\\bprivate\\b" << "\\btrue\\b" << "\\bexport\\b"
             << "\\bprotected\\b" << "\\btry\\b" << "\\bextern\\b" << "\\bpublic\\b" << "\\bcatch\\b"
             << "\\bfalse\\b" << "\\bregister\\b" << "\\btypeid\\b" << "\\breinterpret_cast\\b"
             << "\\btypename\\b" << "\\bclass\\b" << "\\bfriend\\b" << "\\bconst_cast\\b" << "\\bvirtual\\b"
             << "\\binline\\b" << "\\bdelete\\b" << "\\bstatic_cast\\b" << "\\bvolatile\\b"
             << "\\bwchar_t\\b" << "\\bmutable\\b" << "\\bdynamic_cast\\b" << "\\bnamespace\\b" << "\\btemplate\\b";

    languageToKeywordMap[Language::CPP] = keywords;
    keywords.clear();

    keywords << "\\babstract\\b" << "\\bassert\\b" << "\\bboolean\\b" << "\\bbreak\\b" << "\\bbyte\\b"
             << "\\bcase\\b" << "\\bcatch\\b" << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b" << "\\bcontinue\\b"
             << "\\bdefault\\b" << "\\bdo\\b" << "\\bdouble\\b" << "\\belse\\b" << "\\benum\\b" << "\\bextends\\b"
             << "\\bfinal\\b" << "\\bfinally\\b" << "\\bfloat\\b" << "\\bfor\\b" << "\\bgoto\\b" << "\\bif\\b"
             << "\\bimplements\\b" << "\\bimport\\b" << "\\binstanceof\\b" << "\\bint\\b" << "\\binterface\\b"
             << "\\blong\\b" << "\\bnative\\b" << "\\bnew\\b" << "\\bpackage\\b" << "\\bprivate\\b" << "\\bprotected\\b"
             << "\\bpublic\\b" << "\\breturn\\b" << "\\bshort\\b" << "\\bstatic\\b" << "\\bstrictfp\\b" << "\\bsuper\\b"
             << "\\bswitch\\b" << "\\bsynchronized\\b" << "\\bthis\\b" << "\\bthrow\\b" << "\\bthrows\\b" << "\\btransient\\b"
             << "\\btry\\b" << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bwhile\\b" << "\\btrue\\b" << "\\bfalse\\b" << "\\bnull\\b";

    languageToKeywordMap[Language::Java] = keywords;
    keywords.clear();

    keywords << "\\band\\b" << "\\bas\\b" << "\\bassert\\b" << "\\bbreak\\b" << "\\bclass\\b" << "\\bcontinue\\b"
             << "\\bdef\\b" << "\\bdel\\b" << "\\belif\\b" << "\\belse\\b" << "\\bexcept\\b" << "\\bFalse\\b"
             << "\\bfinally\\b" << "\\bfor\\b" << "\\bfrom\\b" << "\\bglobal\\b" << "\\bif\\b" << "\\bimport\\b"
             << "\\bin\\b" << "\\bis\\b" << "\\blambda\\b" << "\\bNone\\b" << "\\bnonlocal\\b" << "\\bnot\\b"
             << "\\bor\\b" << "\\bpass\\b" << "\\braise\\b" << "\\breturn\\b" << "\\bTrue\\b" << "\\btry\\b"
             << "\\bwhile\\b" << "\\bwith\\b" << "\\byield\\b";
    languageToKeywordMap[Language::Python] = keywords;
}


/* Given a Language enum, this function checks the corresponding radio option from the Format > Language
 * menu. Used by on_currentTab_changed to reflect the current tab's selected language.
 */
void MainWindow::triggerCorrespondingMenuLanguageOption(Language lang)
{
    if(lang == Language::C)
    {
        ui->actionC_Lang->trigger();
    }
    else if(lang == Language::CPP)
    {
        ui->actionCPP->trigger();
    }
    else if(lang == Language::Java)
    {
        ui->actionJava_Lang->trigger();
    }
    else
    {
        ui->actionPython_Lang->trigger();
    }
}


/* Called each time the current tab changes in the tabbed editor. Sets the main window's current editor,
 * reconnects any relevant signals, and updates the window.
 */
void MainWindow::on_currentTab_changed(int index)
{
    // Happens when the tabbed editor's last tab is closed
    if(index == -1)
    {
        return;
    }

    // Note: editor will only be nullptr on the first launch, so this will get skipped in that edge case
    if(editor != nullptr)
    {
        // Disconnect for previous active editor
        disconnect(findDialog, SIGNAL(startFinding(QString, bool, bool)), editor, SLOT(find(QString, bool, bool)));
        disconnect(findDialog, SIGNAL(startReplacing(QString, QString, bool, bool)), editor, SLOT(replace(QString, QString, bool, bool)));
        disconnect(findDialog, SIGNAL(startReplacingAll(QString, QString, bool, bool)), editor, SLOT(replaceAll(QString, QString, bool, bool)));
        disconnect(gotoDialog, SIGNAL(gotoLine(int)), editor, SLOT(goTo(int)));
        disconnect(editor, SIGNAL(findResultReady(QString)), findDialog, SLOT(onFindResultReady(QString)));
        disconnect(editor, SIGNAL(gotoResultReady(QString)), gotoDialog, SLOT(onGotoResultReady(QString)));
        disconnect(editor, SIGNAL(undoAvailable(bool)), this, SLOT(toggleUndo(bool)));
        disconnect(editor, SIGNAL(redoAvailable(bool)), this, SLOT(toggleRedo(bool)));
        disconnect(editor, SIGNAL(copyAvailable(bool)), this, SLOT(toggleCopyAndCut(bool)));
    }

    // Set the internal editor to the currently tabbed one
    editor = qobject_cast<Editor*>(tabbedEditor->widget(index));
    editor->setFocus(Qt::FocusReason::TabFocusReason);

    Language tabLanguage = editor->getProgrammingLanguage();

    // If this tab had a programming language set, give it a syntax highlighter
    if(tabLanguage != Language::None)
    {
        triggerCorrespondingMenuLanguageOption(tabLanguage);
        QStringList keywordPatterns = languageToKeywordMap[tabLanguage];
        delete syntaxHighlighter;
        syntaxHighlighter = new Highlighter(keywordPatterns, editor->document());
    }
    else
    {
        // If a menu language is checked but the current tab has no language set, uncheck the menu option
        if(languageGroup->checkedAction())
        {
            languageGroup->checkedAction()->setChecked(false);
        }
    }

    // Reconnect editor signals
    connect(editor, SIGNAL(columnCountChanged(int)), this, SLOT(updateColumnCount(int)));
    connect(editor, SIGNAL(windowNeedsToBeUpdated(DocumentMetrics)), this, SLOT(updateWordAndCharCount(DocumentMetrics)));

    // Update main window actions to reflect the current tab's available actions
    toggleRedo(editor->redoAvailable());
    toggleUndo(editor->undoAvailable());
    toggleCopyAndCut(editor->textCursor().hasSelection());

    // Reconnect editor signals and slots
    connect(editor, SIGNAL(findResultReady(QString)), findDialog, SLOT(onFindResultReady(QString)));
    connect(editor, SIGNAL(gotoResultReady(QString)), gotoDialog, SLOT(onGotoResultReady(QString)));
    connect(editor, SIGNAL(undoAvailable(bool)), this, SLOT(toggleUndo(bool)));
    connect(editor, SIGNAL(redoAvailable(bool)), this, SLOT(toggleRedo(bool)));
    connect(editor, SIGNAL(copyAvailable(bool)), this, SLOT(toggleCopyAndCut(bool)));

    // Reconnect find/goto signals and slots to the current editor
    connect(findDialog, SIGNAL(startFinding(QString, bool, bool)), editor, SLOT(find(QString, bool, bool)));
    connect(findDialog, SIGNAL(startReplacing(QString, QString, bool, bool)), editor, SLOT(replace(QString, QString, bool, bool)));
    connect(findDialog, SIGNAL(startReplacingAll(QString, QString, bool, bool)), editor, SLOT(replaceAll(QString, QString, bool, bool)));
    connect(gotoDialog, SIGNAL(gotoLine(int)), editor, SLOT(goTo(int)));

    // This info only gets passed on by Editor when its contents change, not when a new tab is added to TabbedEditor
    DocumentMetrics metrics = editor->getDocumentMetrics();
    updateWordAndCharCount(metrics);
    updateColumnCount(metrics.currentColumn);
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


/* Launches the Find dialog box if it isn't already visible and sets its focus.
 */
void MainWindow::launchFindDialog()
{
    if(findDialog->isHidden())
    {
        findDialog->show();
        findDialog->activateWindow();
        findDialog->raise();
        findDialog->setFocus();
    }
}


/* Launches the Go To dialog box if it isn't already visible and sets its focus.
 */
void MainWindow::launchGotoDialog()
{
    if(gotoDialog->isHidden())
    {
        gotoDialog->show();
        gotoDialog->activateWindow();
        gotoDialog->raise();
        gotoDialog->setFocus();
    }
}


/* Updates the window and status bar labels to reflect the most up-to-date word and char counts.
 * Note: updating the column count is handled separately. See updateColumnCount in mainwindow.h.
 */
void MainWindow::updateWordAndCharCount(DocumentMetrics metrics)
{
    QString wordText = QString::number(metrics.wordCount) + tr("   ");
    QString charText = QString::number(metrics.charCount) + tr("   ");

    wordCountLabel->setText(wordText);
    charCountLabel->setText(charText);

    QString fileName = editor->getFileName();
    bool editorUnsaved = editor->isUnsaved();

    tabbedEditor->setTabText(tabbedEditor->currentIndex(), fileName + (editorUnsaved ? " *" : ""));
    setWindowTitle(fileName + (editorUnsaved ? " [Unsaved]" : ""));
}


/* Launches a dialog box asking the user if they would like to save the current file.
 * If the user selects "No" or closes the dialog window, the file will not be saved.
 * Otherwise, if they select "Yes," the file will be saved.
 */
QMessageBox::StandardButton MainWindow::askUserToSave()
{
    QString fileName = editor->getFileName();

    return Utility::promptYesOrNo(this, "Unsaved changes", tr("Do you want to save the changes to ") + fileName + tr("?"));
}


/* Called when the user selects the New option from the menu or toolbar (or uses Ctrl+N).
 * Adds a new tab to the editor.
 */
void MainWindow::on_actionNew_triggered()
{
    tabbedEditor->add(new Editor());
    editor->toggleWrapMode(ui->actionWord_Wrap->isChecked());
    editor->toggleAutoIndent(ui->actionAuto_Indent->isChecked());
}


/* Called when the user selects the Save or Save As option from the menu or toolbar
 * (or uses Ctrl+S). On success, saves the contents of the text editor to the disk using
 * the file name provided by the user. If the current document was never saved, or if the
 * user chose Save As, the program prompts the user to specify a name and directory for the file.
 * Returns true if the file was saved and false otherwise.
 */
bool MainWindow::on_actionSave_or_actionSaveAs_triggered()
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
            return false;
        }
        editor->setCurrentFilePath(filePath);
    }

    // Attempt to create a file descriptor with the given path
    QFile file(editor->getCurrentFilePath());
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return false;
    }

    ui->statusBar->showMessage("Document saved", 2000);

    // Save the contents of the editor to the disk and close the file descriptor
    QTextStream out(&file);
    QString editorContents = editor->toPlainText();
    out << editorContents;
    file.close();

    QString fileName = editor->getFileName();
    editor->setModifiedState(false);
    tabbedEditor->setTabText(tabbedEditor->currentIndex(), fileName);
    setWindowTitle(fileName);

    return true;
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

    editor->setModifiedState(false);
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


/* Called when the user tries to close a tab in the editor (or uses Ctrl+W). Allows the user
 * to save the contents of the tab if unsaved. Closes the tab, unless the file is unsaved
 * and the user declines saving. Returns true if the tab was closed and false otherwise.
 */
bool MainWindow::closeTab(int index)
{
    Editor *currentTab = editor;
    Editor *tabToClose = qobject_cast<Editor*>(tabbedEditor->widget(index));
    bool closingCurrentTab = (tabToClose == currentTab);

    // Allow the user to see what tab they're closing if it's not the current one
    if(!closingCurrentTab)
    {
        tabbedEditor->setCurrentWidget(tabToClose);
    }

    if(tabToClose->isUnsaved())
    {
        QMessageBox::StandardButton selection = askUserToSave();

        if(selection == QMessageBox::StandardButton::Yes)
        {
            bool fileSaved = on_actionSave_or_actionSaveAs_triggered();

            if(!fileSaved)
            {
                return false;
            }
        }

        else if(selection == QMessageBox::Cancel)
        {
            return false;
        }
    }

    tabbedEditor->removeTab(index);

    // If we closed the last tab, make a new one
    if(tabbedEditor->count() == 0)
    {
        on_actionNew_triggered();
    }

    // And finally, reset the current tab if needed
    if(!closingCurrentTab)
    {
        tabbedEditor->setCurrentWidget(currentTab);
    }

    return true;
}


/* Called when the user selects the Exit option from the menu. Allows the user
 * to save any unsaved files before quitting.
 */
void MainWindow::on_actionExit_triggered()
{
    // TODO change condition to while true
    while(tabbedEditor->count() != 0)
    {
        bool closed = closeTab(0);

        if(!closed)
        {
            return;
        }

        // The only time this will happen after a tab close is
        // if the last one is closed and a new tab is automatically created
        if(tabbedEditor->count() == 1 && !qobject_cast<Editor*>(tabbedEditor->currentWidget())->isUnsaved())
        {
            break;
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
void MainWindow::on_actionFind_triggered() { launchFindDialog(); }


/* Called when the user explicitly selects the Go To option from the menu (or uses Ctrl+G).
 * Launches a Go To dialog that prompts the user to enter a line number they wish to jump to.
 */
void MainWindow::on_actionGo_To_triggered() { launchGotoDialog(); }


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
 * Toggles auto indenting in all open tabs.
 */
void MainWindow::on_actionAuto_Indent_triggered()
{
    for(int i = 0; i < tabbedEditor->count(); i++)
    {
        Editor *tab = qobject_cast<Editor*>(tabbedEditor->widget(i));
        tab->toggleAutoIndent(ui->actionAuto_Indent->isChecked());
    }
}


/* Called when the user selects the Word Wrap option from the Format menu. Toggles
 * word wrapping in all open tabs.
 */
void MainWindow::on_actionWord_Wrap_triggered()
{
    for(int i = 0; i < tabbedEditor->count(); i++)
    {
        Editor *tab = qobject_cast<Editor*>(tabbedEditor->widget(i));
        tab->toggleWrapMode(ui->actionWord_Wrap->isChecked());
    }
}


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

