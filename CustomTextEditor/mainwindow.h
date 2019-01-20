#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "documentmetrics.h"
#include "editor.h"
#include "finddialog.h"
#include "gotodialog.h"
#include "tabbededitor.h"
#include <QMainWindow>
#include <QCloseEvent>                  // closeEvent
#include <QLabel>                       // GUI labels

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void initializeStatusBarLabels();
    void launchFindDialog();
    void launchGotoDialog();
    void closeEvent(QCloseEvent *event);

private:
    QMessageBox::StandardButton allowUserToSave();

    Ui::MainWindow *ui;
    TabbedEditor *tabbedEditor;
    Editor *editor;
    FindDialog *findDialog;
    GotoDialog *gotoDialog;
    QLabel *wordLabel;
    QLabel *wordCountLabel;
    QLabel *charLabel;
    QLabel *charCountLabel;
    QLabel *columnLabel;
    QLabel *columnCountLabel;

public slots:
    void updateWindow(DocumentMetrics metrics);
    void toggleUndo(bool undoAvailable);
    void toggleRedo(bool redoAvailable);
    void toggleCopyAndCut(bool copyCutAvailable);
    void closeTab(int index);

private slots:
    void on_currentTab_changed(int index);
    void on_actionNew_triggered();
    void on_actionSave_or_actionSaveAs_triggered();
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
};

#endif // MAINWINDOW_H
