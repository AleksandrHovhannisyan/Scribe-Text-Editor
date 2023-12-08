#include "mainwindow.h"
#include "utilityfunctions.h"
#include "ui_mainwindow.h"
#include "settings.h"                   // storing app state
#include <QtDebug>
#include <QtPrintSupport/QPrinter>      // printing
#include <QtPrintSupport/QPrintDialog>  // printing
#include <QFileDialog>                  // file open/save dialogs
#include <QFile>                        // file descriptors, IO
#include <QTextStream>                  // file IO
#include <QStandardPaths>               // default open directory
#include <QDateTime>                    // current time
#include <QApplication>                 // quit
#include <QShortcut>
#include <QProcess>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "deviceinputdialog.h"


/* Sets up the main application window and all of its children/widgets.
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    readSettings();

    // Used to ensure that only one language can ever be checked at a time
    languageGroup = new QActionGroup(this);
    languageGroup->setExclusive(true);
    languageGroup->addAction(ui->actionC_Lang);
    languageGroup->addAction(ui->actionCPP_Lang);
    languageGroup->addAction(ui->actionJava_Lang);
    languageGroup->addAction(ui->actionPython_Lang);
    connect(languageGroup, SIGNAL(triggered(QAction*)), this, SLOT(on_languageSelected(QAction*)));
    // Language label frame
    setupLanguageOnStatusBar();

    // Set up the find dialog
    findDialog = new FindDialog();
    findDialog->setParent(this, Qt::Tool | Qt::MSWindowsFixedSizeDialogHint);

    // Set up the goto dialog
    gotoDialog = new GotoDialog();
    gotoDialog->setParent(this, Qt::Tool | Qt::MSWindowsFixedSizeDialogHint);

    // Set up the tabbed editor
    tabbedEditor = ui->tabWidget;
    tabbedEditor->setTabsClosable(true);

    // Add metric reporter and simulate a tab switch
    metricReporter = new MetricReporter();
    ui->statusBar->addPermanentWidget(metricReporter);
    on_currentTabChanged(0);

    // Connect tabbedEditor's signals to their handlers
    connect(tabbedEditor, SIGNAL(currentChanged(int)), this, SLOT(on_currentTabChanged(int)));
    connect(tabbedEditor, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));

    // Connect action signals to their handlers
    connect(ui->actionSave, SIGNAL(triggered()), this, SLOT(on_actionSaveTriggered()));
    connect(ui->actionSave_As, SIGNAL(triggered()), this, SLOT(on_actionSaveTriggered()));
    connect(ui->actionReplace, SIGNAL(triggered()), this, SLOT(on_actionFind_triggered()));

    // Have to add this shortcut manually because we can't define it via the GUI editor
    QShortcut *tabCloseShortcut = new QShortcut(QKeySequence("Ctrl+W"), this);
    QObject::connect(tabCloseShortcut, SIGNAL(activated()), this, SLOT(closeTabShortcut()));

    // For word wrap and auto indent
    matchFormatOptionsToEditorDefaults();

    // Connect Compile and Upload buttons to slots
    connect(ui->actionCompileRiscV, SIGNAL(triggered()), this, SLOT(on_actionCompileRiscVTriggered()));
    connect(ui->actionUploadToSCMB, SIGNAL(triggered()), this, SLOT(on_actionUploadToSCMBTriggered()));

    mapMenuLanguageOptionToLanguageType();
    mapFileExtensionsToLanguages();
    appendShortcutsToToolbarTooltips();
}


/* Ensures that the checkable formatting menu options, like auto indent
 * and word wrap, match the previously saved defaults for the Editor class.
 * See constructor for usage.
 */
void MainWindow::matchFormatOptionsToEditorDefaults()
{
    QAction *autoIndent = ui->actionAuto_Indent;
    editor->autoIndentEnabled ? autoIndent->setChecked(true) : autoIndent->setChecked(false);

    QAction *wordWrap = ui->actionWord_Wrap;
    editor->lineWrapMode ? wordWrap->setChecked(true) : wordWrap->setChecked(false);
}


/* Updates the Format menu options (e.g., Word wrap, Auto indent) to match
 * the settings of the currently selected editor. See onCurrentTabChanged for usage.
 */
void MainWindow::updateFormatMenuOptions()
{
    ui->actionWord_Wrap->setChecked(editor->textIsWrapped());
    ui->actionAuto_Indent->setChecked(editor->textIsAutoIndented());
}


/* Initializes the language label and adds it to a frame
 * that gets set as a widget on the far left of the status bar.
 */
void MainWindow::setupLanguageOnStatusBar()
{
    languageLabel = new QLabel("Language: not selected");
    QFrame *langFrame = new QFrame();
    QHBoxLayout *langLayout = new QHBoxLayout();
    langLayout->addWidget(languageLabel);
    langFrame->setLayout(langLayout);
    ui->statusBar->addWidget(langFrame);
}


/* Maps each menu language option (from the Format dropdown) to its corresponding
 * Language type, for convenience.
 */
void MainWindow::mapMenuLanguageOptionToLanguageType()
{
    menuActionToLanguageMap[ui->actionC_Lang] = Language::C;
    menuActionToLanguageMap[ui->actionCPP_Lang] = Language::CPP;
    menuActionToLanguageMap[ui->actionJava_Lang] = Language::Java;
    menuActionToLanguageMap[ui->actionPython_Lang] = Language::Python;
}


/* Maps known file extensions to the languages the editor supports.
 */
void MainWindow::mapFileExtensionsToLanguages()
{
    extensionToLanguageMap.insert("cpp", Language::CPP);
    extensionToLanguageMap.insert("h", Language::CPP);
    extensionToLanguageMap.insert("c", Language::C);
    extensionToLanguageMap.insert("java", Language::Java);
    extensionToLanguageMap.insert("py", Language::Python);
}


void MainWindow::appendShortcutsToToolbarTooltips()
{
    for (QAction* action : ui->mainToolBar->actions())
    {
        QString tooltip = action->toolTip() + " (" + action->shortcut().toString() + ")";
        action->setToolTip(tooltip);
    }
}

/* Performs all necessary memory cleanup operations on dynamically allocated objects.
 */
MainWindow::~MainWindow()
{
    delete languageLabel;
    delete languageGroup;
    delete ui;
}


/* Called when the user selects a language from the main menu. Sets the current language to
 * that language internally for the currently tabbed Editor.
 */
void MainWindow::on_languageSelected(QAction* languageAction)
{
    Language language = menuActionToLanguageMap[languageAction];
    selectProgrammingLanguage(language);
}


/* Given a Language enum, this function checks the corresponding radio option from the Format > Language
 * menu. Used by on_currentTabChanged to reflect the current tab's selected language.
 */
void MainWindow::triggerCorrespondingMenuLanguageOption(Language lang)
{
    switch (lang)
    {
        case (Language::C):
            if (!ui->actionC_Lang->isChecked())
            {
                ui->actionC_Lang->trigger();
            }
            break;

        case (Language::CPP):
            if (!ui->actionCPP_Lang->isChecked())
            {
                ui->actionCPP_Lang->trigger();
            }
            break;

        case (Language::Java):
            if (!ui->actionJava_Lang->isChecked())
            {
                ui->actionJava_Lang->trigger();
            }
            break;

        case (Language::Python):
            if (!ui->actionPython_Lang->isChecked())
            {
                ui->actionPython_Lang->trigger();
            }
            break;

        default: return;
    }
}


/* Uses the extension of a file to determine what language, if any, it should be
 * mapped to. If the extension does not match one of the supported languages, or if
 * the file does not have an extension, then the language is set to Language::None.
 */
void MainWindow::setLanguageFromExtension()
{
    QString fileName = editor->getFileName();
    int indexOfDot = fileName.indexOf('.');

    if (indexOfDot == -1)
    {
        selectProgrammingLanguage(Language::None);
        return;
    }

    QString fileExtension = fileName.mid(indexOfDot + 1);

    bool extensionSupported = extensionToLanguageMap.find(fileExtension) != extensionToLanguageMap.end();

    if (!extensionSupported)
    {
        selectProgrammingLanguage(Language::None);
        return;
    }

    selectProgrammingLanguage(extensionToLanguageMap[fileExtension]);
}


/* Wrapper for all common logic that needs to run whenever a given language
 * is selected for use on a particular tab. Triggers the corresponding menu option.
 */
void MainWindow::selectProgrammingLanguage(Language language)
{
    if (language == editor->getProgrammingLanguage())
    {
        return;
    }

    editor->setProgrammingLanguage(language);
    languageLabel->setText(toString(language));
    triggerCorrespondingMenuLanguageOption(language);
}


/* Disconnects all signals that depend on the cached editor/tab. Used mainly
 * when the current editor is changed (when a new tab is opened, for example).
 */
void MainWindow::disconnectEditorDependentSignals()
{
    disconnect(findDialog, SIGNAL(startFinding(QString, bool, bool)), editor, SLOT(find(QString, bool, bool)));
    disconnect(findDialog, SIGNAL(startReplacing(QString, QString, bool, bool)), editor, SLOT(replace(QString, QString, bool, bool)));
    disconnect(findDialog, SIGNAL(startReplacingAll(QString, QString, bool, bool)), editor, SLOT(replaceAll(QString, QString, bool, bool)));
    disconnect(gotoDialog, SIGNAL(gotoLine(int)), editor, SLOT(goTo(int)));
    disconnect(editor, SIGNAL(findResultReady(QString)), findDialog, SLOT(onFindResultReady(QString)));
    disconnect(editor, SIGNAL(gotoResultReady(QString)), gotoDialog, SLOT(onGotoResultReady(QString)));

    disconnect(editor, SIGNAL(wordCountChanged(int)), metricReporter, SLOT(updateWordCount(int)));
    disconnect(editor, SIGNAL(charCountChanged(int)), metricReporter, SLOT(updateCharCount(int)));
    disconnect(editor, SIGNAL(lineCountChanged(int, int)), metricReporter, SLOT(updateLineCount(int, int)));
    disconnect(editor, SIGNAL(columnCountChanged(int)), metricReporter, SLOT(updateColumnCount(int)));
    disconnect(editor, SIGNAL(fileContentsChanged()), this, SLOT(updateTabAndWindowTitle()));

    disconnect(editor, SIGNAL(undoAvailable(bool)), this, SLOT(toggleUndo(bool)));
    disconnect(editor, SIGNAL(redoAvailable(bool)), this, SLOT(toggleRedo(bool)));
    disconnect(editor, SIGNAL(copyAvailable(bool)), this, SLOT(toggleCopyAndCut(bool)));
}


/* Connects all signals and slots that depend on the cached editor/tab. Used mainly
 * when the current editor is changed (when a new tab is opened, for example).
 */
void MainWindow::reconnectEditorDependentSignals()
{
    connect(findDialog, SIGNAL(startFinding(QString, bool, bool)), editor, SLOT(find(QString, bool, bool)));
    connect(findDialog, SIGNAL(startReplacing(QString, QString, bool, bool)), editor, SLOT(replace(QString, QString, bool, bool)));
    connect(findDialog, SIGNAL(startReplacingAll(QString, QString, bool, bool)), editor, SLOT(replaceAll(QString, QString, bool, bool)));
    connect(gotoDialog, SIGNAL(gotoLine(int)), editor, SLOT(goTo(int)));
    connect(editor, SIGNAL(findResultReady(QString)), findDialog, SLOT(onFindResultReady(QString)));
    connect(editor, SIGNAL(gotoResultReady(QString)), gotoDialog, SLOT(onGotoResultReady(QString)));

    connect(editor, SIGNAL(wordCountChanged(int)), metricReporter, SLOT(updateWordCount(int)));
    connect(editor, SIGNAL(charCountChanged(int)), metricReporter, SLOT(updateCharCount(int)));
    connect(editor, SIGNAL(lineCountChanged(int, int)), metricReporter, SLOT(updateLineCount(int, int)));
    connect(editor, SIGNAL(columnCountChanged(int)), metricReporter, SLOT(updateColumnCount(int)));
    connect(editor, SIGNAL(fileContentsChanged()), this, SLOT(updateTabAndWindowTitle()));

    connect(editor, SIGNAL(undoAvailable(bool)), this, SLOT(toggleUndo(bool)));
    connect(editor, SIGNAL(redoAvailable(bool)), this, SLOT(toggleRedo(bool)));
    connect(editor, SIGNAL(copyAvailable(bool)), this, SLOT(toggleCopyAndCut(bool)));
}


/* Called each time the current tab changes in the tabbed editor. Sets the main window's current editor,
 * reconnects any relevant signals, and updates the window.
 */
void MainWindow::on_currentTabChanged(int index)
{
    // Happens when the tabbed editor's last tab is closed
    if (index == -1)
    {
        return;
    }

    // Note: editor will only be nullptr on the first launch, so this will get skipped in that edge case
    if (editor != nullptr)
    {
        disconnectEditorDependentSignals();
    }

    editor = tabbedEditor->currentTab();
    reconnectEditorDependentSignals();
    editor->setFocus(Qt::FocusReason::TabFocusReason);

    Language tabLanguage = editor->getProgrammingLanguage();

    // If this tab had a programming language set, trigger the corresponding option
    if (tabLanguage != Language::None)
    {
        triggerCorrespondingMenuLanguageOption(tabLanguage);
    }
    else
    {        
        // If a menu language is checked but the current tab has no language set, uncheck the menu option
        if (languageGroup->checkedAction())
        {
            languageGroup->checkedAction()->setChecked(false);
        }
    }

    // Update language reflected on status bar
    languageLabel->setText(toString(tabLanguage));

    // Update main window actions to reflect the current tab's available actions
    toggleRedo(editor->redoAvailable());
    toggleUndo(editor->undoAvailable());
    toggleCopyAndCut(editor->textCursor().hasSelection());

    updateFormatMenuOptions();


    // We need to update this information manually for tab changes
    DocumentMetrics metrics = editor->getDocumentMetrics();
    updateTabAndWindowTitle();
    metricReporter->updateWordCount(metrics.wordCount);
    metricReporter->updateCharCount(metrics.charCount);
    metricReporter->updateLineCount(metrics.currentLine, metrics.totalLines);
    metricReporter->updateColumnCount(metrics.currentColumn);
}


/* Launches the Find dialog box if it isn't already visible and sets its focus.
 */
void MainWindow::launchFindDialog()
{
    if (findDialog->isHidden())
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
    if (gotoDialog->isHidden())
    {
        gotoDialog->show();
        gotoDialog->activateWindow();
        gotoDialog->raise();
        gotoDialog->setFocus();
    }
}


/* Updates the tab name and the main application window title to reflect the
 * currently open document.
 */
void MainWindow::updateTabAndWindowTitle()
{
    QString tabTitle = editor->getFileName();
    QString windowTitle = tabTitle;

    if (editor->isUnsaved())
    {
        tabTitle += " *";
        windowTitle += " [Unsaved]";
    }

    tabbedEditor->setTabText(tabbedEditor->currentIndex(), tabTitle);
    setWindowTitle(windowTitle + " - Scribe");
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
}


/* Called when the user selects the Save or Save As option from the menu or toolbar
 * (or uses Ctrl+S). On success, saves the contents of the text editor to the disk using
 * the file name provided by the user. If the current document was never saved, or if the
 * user chose Save As, the program prompts the user to specify a name and directory for the file.
 * Returns true if the file was saved and false otherwise.
 */
bool MainWindow::on_actionSaveTriggered()
{
    bool saveAs = sender() == ui->actionSave_As;
    QString currentFilePath = editor->getCurrentFilePath();

    // If user hit Save As or user hit Save but current document was never saved to disk
    if (saveAs || currentFilePath.isEmpty())
    {
        // Title to be used for saving dialog
        QString saveDialogWindowTitle = saveAs ? tr("Save As") : tr("Save");

        // Try to get a valid file path
        QString filePath = QFileDialog::getSaveFileName(this, saveDialogWindowTitle);

        // Don't do anything if the user changes their mind and hits Cancel
        if (filePath.isNull())
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

    editor->setModifiedState(false);
    updateTabAndWindowTitle();
    setLanguageFromExtension();

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
    // Used to switch to a new tab if there's already an open doc
    bool openInCurrentTab = editor->isUntitled() && !editor->isUnsaved();

    QString openedFilePath;
    QString lastUsedDirectory = settings->value(DEFAULT_DIRECTORY_KEY).toString();

    if (lastUsedDirectory.isEmpty())
    {
        openedFilePath = QFileDialog::getOpenFileName(this, tr("Open"), DEFAULT_DIRECTORY);
    }
    else
    {
        openedFilePath = QFileDialog::getOpenFileName(this, tr("Open"), lastUsedDirectory);
    }

    // Don't do anything if the user hit Cancel
    if (openedFilePath.isNull())
    {
        return;
    }

    // Update the recently used directory
    QDir currentDirectory;
    settings->setValue(DEFAULT_DIRECTORY_KEY, currentDirectory.absoluteFilePath(openedFilePath));

    // Attempt to create a file descriptor for the file at the given path
    QFile file(openedFilePath);
    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    // Read the file contents into the editor and close the file descriptor
    QTextStream in(&file);
    QString documentContents = in.readAll();

    if (!openInCurrentTab)
    {
        tabbedEditor->add(new Editor());
    }
    editor->setCurrentFilePath(openedFilePath);
    editor->setPlainText(documentContents);
    file.close();

    editor->setModifiedState(false);
    updateTabAndWindowTitle();
    setLanguageFromExtension();
}


/* Called when the user selects the Print option from the menu or toolbar (or uses Ctrl+P).
 * Allows the user to print the contents of the current document.
 */
void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer;
    printer.setPrinterName(tr("Document printer"));
    QPrintDialog printDialog(&printer, this);

    if (printDialog.exec() != QPrintDialog::Rejected)
    {
        editor->print(&printer);
        ui->statusBar->showMessage("Printing", 2000);
    }
}


/* Called when the user tries to close a tab in the editor (or uses Ctrl+W). Allows the user
 * to save the contents of the tab if unsaved. Closes the tab, unless the file is unsaved
 * and the user declines saving. Returns true if the tab was closed and false otherwise.
 */
bool MainWindow::closeTab(Editor *tabToClose)
{
    Editor *currentTab = editor;
    bool closingCurrentTab = (tabToClose == currentTab);

    // Allow the user to see what tab they're closing if it's not the current one
    if (!closingCurrentTab)
    {
        tabbedEditor->setCurrentWidget(tabToClose);
    }

    // Don't close a tab immediately if it has unsaved contents
    if (tabToClose->isUnsaved())
    {
        QMessageBox::StandardButton selection = askUserToSave();

        if (selection == QMessageBox::StandardButton::Yes)
        {
            bool fileSaved = on_actionSaveTriggered();

            if (!fileSaved)
            {
                return false;
            }
        }

        else if (selection == QMessageBox::Cancel)
        {
            return false;
        }
    }

    int indexOfTabToClose = tabbedEditor->indexOf(tabToClose);
    tabbedEditor->removeTab(indexOfTabToClose);

    // If we closed the last tab, make a new one
    if (tabbedEditor->count() == 0)
    {
        on_actionNew_triggered();
    }

    // And finally, go back to original tab if the user was closing a different one
    if (!closingCurrentTab)
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
    QVector<Editor*> unsavedTabs = tabbedEditor->unsavedTabs();

    for (Editor *tab : unsavedTabs)
    {
        bool userClosedTab = closeTab(tab);

        if (!userClosedTab)
        {
            return;
        }
    }

    writeSettings();
    QApplication::quit();
}


/* Saves the main application state/settings so they may be
 * restored the next time the application is started. See
 * readSettings and the constructor for more info.
 */
void MainWindow::writeSettings()
{
    settings->setValue(WINDOW_SIZE_KEY, size());
    settings->setValue(WINDOW_POSITION_KEY, pos());
    settings->setValue(WINDOW_STATUS_BAR, ui->statusBar->isVisible());
    settings->setValue(WINDOW_TOOL_BAR, ui->mainToolBar->isVisible());
}


/* Reads the stored app settings and restores them.
 */
void MainWindow::readSettings()
{
    settings->apply(settings->value(WINDOW_SIZE_KEY, QSize(400, 400)),
                    [=](QVariant setting){ this->resize(setting.toSize()); });

    settings->apply(settings->value(WINDOW_POSITION_KEY, QPoint(200, 200)),
                    [=](QVariant setting){ this->move(setting.toPoint()); });

    settings->apply(settings->value(WINDOW_STATUS_BAR),
                    [=](QVariant setting) {
                        this->ui->statusBar->setVisible(qvariant_cast<bool>(setting));
                        this->ui->actionStatus_Bar->setChecked(qvariant_cast<bool>(setting));
                    });

    settings->apply(settings->value(WINDOW_TOOL_BAR),
                    [=](QVariant setting) {
                        this->ui->mainToolBar->setVisible(qvariant_cast<bool>(setting));
                        this->ui->actionTool_Bar->setChecked(qvariant_cast<bool>(setting));
                    });
}


/* Called when the Undo operation is toggled by the editor.
 */
void MainWindow::toggleUndo(bool undoAvailable)
{
    ui->actionUndo->setEnabled(undoAvailable);
}


/* Called when the Redo operation is toggled by the editor.
 */
void MainWindow::toggleRedo(bool redoAvailable)
{
    ui->actionRedo->setEnabled(redoAvailable);
}


/* Called when the user performs the Undo operation.
 */
void MainWindow::on_actionUndo_triggered()
{
    if (ui->actionUndo->isEnabled())
    {
        editor->undo();
    }
}


/* Called when the user performs the Redo operation.
 */
void MainWindow::on_actionRedo_triggered()
{
    if (ui->actionRedo->isEnabled())
    {
        editor->redo();
    }
}


/* Called when the Copy and Cut operations are toggled by the editor.
 */
void MainWindow::toggleCopyAndCut(bool copyCutAvailable)
{
    ui->actionCopy->setEnabled(copyCutAvailable);
    ui->actionCut->setEnabled(copyCutAvailable);
}


/* Called when the user performs the Cut operation.
 */
void MainWindow::on_actionCut_triggered()
{
    if (ui->actionCut->isEnabled())
    {
        editor->cut();
    }
}


/* Called when the user performs the Copy operation.
 */
void MainWindow::on_actionCopy_triggered()
{
    if (ui->actionCopy->isEnabled())
    {
        editor->copy();
    }
}


/* Called when the user performs the Paste operation.
 */
void MainWindow::on_actionPaste_triggered()
{
    editor->paste();
}


/* Called when the user explicitly selects the Find option from the menu
 * (or uses Ctrl+F). Launches a dialog that prompts the user to enter a search query.
 */
void MainWindow::on_actionFind_triggered()
{
    launchFindDialog();
}


/* Called when the user explicitly selects the Go To option from the menu (or uses Ctrl+G).
 * Launches a Go To dialog that prompts the user to enter a line number they wish to jump to.
 */
void MainWindow::on_actionGo_To_triggered()
{
    launchGotoDialog();
}


/* Called when the user explicitly selects the Select All option from the menu (or uses Ctrl+A).
 */
void MainWindow::on_actionSelect_All_triggered()
{
    editor->selectAll();
}


/* Called when the user explicitly selects the Time/Date option from the menu (or uses F5).
 */
void MainWindow::on_actionTime_Date_triggered()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    editor->insertPlainText(currentTime.toString());
}


/* Called when the user selects the Font option from the menu. Launches a font selection dialog.
 */
void MainWindow::on_actionFont_triggered()
{
    tabbedEditor->promptFontSelection();
}


/* Called when the user selects the Auto Indent option from the Format menu.
 */
void MainWindow::on_actionAuto_Indent_triggered()
{
    bool shouldAutoIndent = ui->actionAuto_Indent->isChecked();
    bool autoIndentToggled = tabbedEditor->applyAutoIndentation(shouldAutoIndent);

    // If the user canceled the operation, reverse the checking
    if (!autoIndentToggled)
    {
        ui->actionAuto_Indent->setChecked(!shouldAutoIndent);
    }
}


/* Called when the user selects the Word Wrap option from the Format menu.
 */
void MainWindow::on_actionWord_Wrap_triggered()
{
    tabbedEditor->applyWordWrapping(ui->actionWord_Wrap->isChecked());
}


/* Toggles the visibility of the given widget. It is assumed that this
 * widget is part of the main window. Otherwise, the effect may not be seen.
 */
void MainWindow::toggleVisibilityOf(QWidget *widget)
{
    bool opposite = !widget->isVisible();
    widget->setVisible(opposite);
}


/* Toggles the visibility of the status bar.
 */
void MainWindow::on_actionStatus_Bar_triggered()
{
    toggleVisibilityOf(ui->statusBar);
}


/* Toggles the visibility of the main tool bar
*/
void MainWindow::on_actionTool_Bar_triggered()
{
    toggleVisibilityOf(ui->mainToolBar);
}


/* Overrides the QWidget closeEvent virtual method. Called when the user tries
 * to close the main application window conventually via the red X. Allows the
 * user to save any unsaved files before quitting.
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    event->ignore();
    on_actionExit_triggered();
}

void MainWindow::on_actionCompileRiscVTriggered()
{
    // Start work directory at no change from process location
    QString work_dir = "";
    QProcess *proc;
    proc = new QProcess(this);
    work_dir = QFileDialog::getExistingDirectory(this, tr("Choose where Make is"), "$HOME", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    proc->setWorkingDirectory(work_dir);
    proc->start("make", {"tetris", "TARGET=scmb"}, QProcess::Unbuffered | QProcess::ReadOnly);
    proc->waitForFinished();
    QString output(proc->readAllStandardOutput());
    QMessageBox compileResults(this);
    compileResults.setText(output);
    compileResults.setWindowTitle("Compilation Results");
    compileResults.exec();
}

void MainWindow::on_actionUploadToSCMBTriggered()
{
    // Get a string which points to the binary of whatever file
    QString bin_file_location = "";
    bin_file_location = QFileDialog::getOpenFileName(this, tr("Choose where the Binary is"), "Binary Files (*.bin)");
    if (bin_file_location.isEmpty())
    {
        // Bin File not found. Warn user
        QMessageBox errorMessage(this);
        errorMessage.setText("Error: File not found.");
        errorMessage.setWindowTitle("Error opening bin file");
        errorMessage.exec();
        return;
    }
    // Load binary using whatever call needs to be made to Isaac.
    // Can be either a QFile or whatever is going to work for Isaac's solution.
    // binary files are raw 1s and 0s. There is a list location that is in the same directory that contains the asm
    QString lst_file_location = bin_file_location.replace(bin_file_location.size()-3, bin_file_location.size(), "lst");
    // open the lst file as the replace does work correctly
    bool openInCurrentTab = editor->isUntitled() && !editor->isUnsaved();
    QFile file(lst_file_location);
    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot open lst file: " + file.errorString());
        return;
    }

    QTextStream in(&file);
    QString documentContents = in.readAll();

    if (!openInCurrentTab)
    {
        tabbedEditor->add(new Editor());
    }
    editor->setCurrentFilePath(lst_file_location);
    editor->setPlainText(documentContents);
    file.close();

    editor->setModifiedState(false);
    updateTabAndWindowTitle();
    setLanguageFromExtension();

    // Get the device name
    QString deviceName = "/dev/ttyACM0";
    QString newDeviceName = DeviceInputDialog::getText();
    if (!newDeviceName.isEmpty())
        deviceName = newDeviceName;
    // Open up a new process calling Isaac's "main"
    QProcess *proc = new QProcess(this);
    if (memoryProcess == nullptr)
    {
        memoryProcess = new MemoryTerminal(nullptr, proc);
    }
    connect(proc, SIGNAL(readyReadStandardOutput()), memoryProcess, SLOT(reportCurrentProcess()));
    memoryProcess->show();
    //
    proc->setWorkingDirectory("~/Desktop/Senior Project/ComputerSideInterface/");
    proc->start("main", {deviceName, bin_file_location}, QProcess::Unbuffered | QProcess::ReadOnly);

}

void MainWindow::reportMessage()
{

}

