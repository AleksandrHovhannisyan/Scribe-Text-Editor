#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QStringRef>
#include <QFont>
#include <QFontMetrics>     // tab stop width

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
    QMessageBox::StandardButton promptYesOrNo(QString title, QString prompt);
    void setTabStopWidth(int width);
    void setFont(QString family, QFont::StyleHint styleHint,
                    bool fixedPitch, int pointSize);
    const QString defaultWindowTitle = "Untitled document";

private:
    QString getFileNameFromPath(QString filePath);
    QFont font;
    int tabStopWidth;
    Ui::MainWindow *ui;
    QString currentFile;
    bool fileNeedsToBeSaved = true;

private slots:
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionOpen_triggered();
    void on_textEdit_textChanged();
};

#endif // MAINWINDOW_H
