#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "documentmetrics.h"
#include "editor.h"
#include "finddialog.h"
#include "gotodialog.h"
#include "tabbededitor.h"
#include "language.h"
#include <highlighter.h>
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
    void initializeStatusBarLabels();
    void launchFindDialog();
    void launchGotoDialog();
    void closeEvent(QCloseEvent *event) override;

private:
    void reconnectEditorDependentSignals();
    void disconnectEditorDependentSignals();
    QMessageBox::StandardButton askUserToSave();

    void selectProgrammingLanguage(Language language);
    void triggerCorrespondingMenuLanguageOption(Language lang);
    void mapMenuLanguageOptionToLanguageType();
    void mapFileExtensionsToLanguages();
    void setLanguageFromExtension();

    void matchFormatOptionsToEditorDefaults();
    void writeSettings();
    void readSettings();

    Ui::MainWindow *ui;
    TabbedEditor *tabbedEditor;
    Editor *editor = nullptr;

    // Used for storing application state upon termination
    const QString WINDOW_SIZE_KEY = "window_size";
    const QString WINDOW_POSITION_KEY = "window_position";
    const QString DEFAULT_DIRECTORY_KEY = "default_directory";
    const QString DEFAULT_DIRECTORY = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);

    FindDialog *findDialog;
    GotoDialog *gotoDialog;
    QActionGroup *languageGroup;
    QMap<QAction*, Language> menuActionToLanguageMap;
    QMap<QString, Language> extensionToLanguageMap;
    QLabel *languageLabel;
    QLabel *wordLabel;
    QLabel *wordCountLabel;
    QLabel *charLabel;
    QLabel *charCountLabel;
    QLabel *columnLabel;
    QLabel *columnCountLabel;

public slots:
    inline void updateColumnCount(int col) { columnCountLabel->setText(QString::number(col) + tr("   ")); }
    void updateTabAndWindowTitle();
    void updateWordAndCharCount(DocumentMetrics metrics);
    void toggleUndo(bool undoAvailable);
    void toggleRedo(bool redoAvailable);
    void toggleCopyAndCut(bool copyCutAvailable);
    bool closeTab(Editor *tabToClose);
    bool closeTab(int index) { return closeTab(tabbedEditor->tabAt(index)); }
    void closeTabShortcut() { closeTab(tabbedEditor->currentTab()); }
    inline void informUser(QString title, QString message) { QMessageBox::information(findDialog, title, message); }

private slots:
    void on_currentTab_changed(int index);
    void on_languageSelected(QAction* languageAction);
    void on_actionNew_triggered();
    bool on_actionSave_or_actionSaveAs_triggered();
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
};

#endif // MAINWINDOW_H
