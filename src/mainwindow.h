#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "documentmetrics.h"
#include "editor.h"
#include "finddialog.h"
#include "gotodialog.h"
#include "memoryterminal.h"
#include "qprocess.h"
#include "tabbededitor.h"
#include "language.h"
#include "metricreporter.h"
#include <highlighters/highlighter.h>
#include <QMainWindow>
#include <QCloseEvent>                  // closeEvent
#include <QLabel>                       // GUI labels
#include <QActionGroup>
#include <QStandardPaths>               // see default directory


using namespace ProgrammingLanguage;


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void launchFindDialog();
    void launchGotoDialog();
    void closeEvent(QCloseEvent *event) override;

private:
    void reconnectEditorDependentSignals();
    void disconnectEditorDependentSignals();
    QMessageBox::StandardButton askUserToSave();

    void appendShortcutsToToolbarTooltips();
    void setupLanguageOnStatusBar();
    void selectProgrammingLanguage(Language language);
    void triggerCorrespondingMenuLanguageOption(Language lang);
    void mapMenuLanguageOptionToLanguageType();
    void mapFileExtensionsToLanguages();
    void setLanguageFromExtension();

    void matchFormatOptionsToEditorDefaults();
    void updateFormatMenuOptions();
    void writeSettings();
    void readSettings();

    void toggleVisibilityOf(QWidget *widget);

    // The "core" or essential members
    Ui::MainWindow *ui;
    TabbedEditor *tabbedEditor;
    MetricReporter *metricReporter;
    Editor *editor = nullptr;
    Settings *settings = Settings::instance();
    QProcess *procptr;

    MemoryTerminal *memoryProcess;

    // Used for storing application state upon termination
    const QString WINDOW_SIZE_KEY = "window_size";
    const QString WINDOW_POSITION_KEY = "window_position";
    const QString WINDOW_STATUS_BAR = "window_status_bar";
    const QString WINDOW_TOOL_BAR = "window_tool_bar";
    const QString DEFAULT_DIRECTORY_KEY = "default_directory";
    const QString DEFAULT_DIRECTORY = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    // Other widget members
    FindDialog *findDialog;
    GotoDialog *gotoDialog;
    QActionGroup *languageGroup;
    QLabel *languageLabel;
    QMap<QAction*, Language> menuActionToLanguageMap;
    QMap<QString, Language> extensionToLanguageMap;

public slots:
    void toggleUndo(bool undoAvailable);
    void toggleRedo(bool redoAvailable);
    void toggleCopyAndCut(bool copyCutAvailable);

    void updateTabAndWindowTitle();
    bool closeTab(Editor *tabToClose);
    inline bool closeTab(int index) { return closeTab(tabbedEditor->tabAt(index)); }
    inline void closeTabShortcut() { closeTab(tabbedEditor->currentTab()); }
    inline void informUser(QString title, QString message) { QMessageBox::information(findDialog, title, message); }
    void reportMessage();

// All UI and/or keyboard shortcut interactions
private slots:
    void on_currentTabChanged(int index);
    void on_languageSelected(QAction* languageAction);
    void on_actionNew_triggered();
    bool on_actionSaveTriggered();
    void on_actionOpen_triggered();
    void on_actionExit_triggered();
    void on_actionUndo_triggered();
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionFind_triggered();
    void on_actionGo_To_triggered();
    void on_actionSelect_All_triggered();
    void on_actionRedo_triggered();
    void on_actionPrint_triggered();
    void on_actionStatus_Bar_triggered();
    void on_actionTime_Date_triggered();
    void on_actionFont_triggered();
    void on_actionAuto_Indent_triggered();
    void on_actionWord_Wrap_triggered();
    void on_actionTool_Bar_triggered();
    void on_actionCompileRiscVTriggered();
    void on_actionUploadToSCMBTriggered();
};

#endif // MAINWINDOW_H
