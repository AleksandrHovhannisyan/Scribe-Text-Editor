#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include "documentmetrics.h"
#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStringRef>
#include <QFont>
#include <QFontMetrics>                 // tab stop width
#include <QApplication>                 // quit
#include <QCloseEvent>                  // closeEvent
#include <QtPrintSupport/QPrinter>      // printing
#include <QtPrintSupport/QPrintDialog>  // printing

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void resetEditor();
    void setTabStopWidth(int width);
    void setFont(QString family, QFont::StyleHint styleHint,
                    bool fixedPitch, int pointSize);
    void closeEvent(QCloseEvent *event);
    const QString defaultWindowTitle = "Untitled document";

private:
    QString getFileNameFromPath(QString filePath);
    void allowUserToSave();
    void updateFileMetrics();
    DocumentMetrics metrics;
    QFont font;
    int tabStopWidth;
    Ui::MainWindow *ui;
    QString currentFilePath;
    bool fileNeedsToBeSaved = true;

private slots:
    void on_actionNew_triggered();
    void on_actionSave_or_actionSaveAs_triggered();
    void on_actionOpen_triggered();
    void on_textEdit_textChanged();
    void on_actionExit_triggered();
    void on_actionUndo_triggered();
    void on_actionCut_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionFind_triggered();
    void on_actionFind_Next_triggered();
    void on_actionReplace_triggered();
    void on_actionGo_To_triggered();
    void on_actionSelect_All_triggered();
    void on_actionRedo_triggered();
    void on_actionPrint_triggered();
};

#endif // MAINWINDOW_H
