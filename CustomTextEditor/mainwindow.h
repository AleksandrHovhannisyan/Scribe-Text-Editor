#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStringRef>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    const QString defaultWindowTitle = "Untitled document";

private:
    Ui::MainWindow *ui;
    QString currentFile;
    bool fileNeedsToBeSaved = true;

    inline QString getFileNameFromPath(QString filePath);

private slots:
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_textEdit_textChanged();
};

#endif // MAINWINDOW_H
