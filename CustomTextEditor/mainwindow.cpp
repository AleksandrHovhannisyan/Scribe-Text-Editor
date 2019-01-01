#include "mainwindow.h"
#include "ui_mainwindow.h"

// TODO ensure we prompt a save before the user exits the application if there's still stuff there!

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resetEditor();
    setFont("Courier", QFont::Monospace, true, 10);
    setTabStopWidth(5);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Resets the text editor to all of its defaults and
 * effectively creates an empty document from scratch.
 * See the constructor and on_actionNew_triggered().
 */
void MainWindow::resetEditor()
{
    currentFile.clear();
    ui->textEdit->setText(QString());
    setWindowTitle(defaultWindowTitle);
    fileNeedsToBeSaved = false;
}

/* Prompts the user to make a yes or no selection from the message box.
 * @param title - the title of the message box window
 * @param prompt - the text prompt/question to show the user
 * @return the user's selection as a QMessageBox::StandardButton (Yes or No).
 */
QMessageBox::StandardButton MainWindow::promptYesOrNo(QString title, QString prompt)
{
    return QMessageBox::question(this, title, prompt, QMessageBox::Yes | QMessageBox::No);
}


/* Sets the editor's tab stop width to the specified value.
 * @param width - the width of the tab stop, in terms of the number of equivalent space characters.
 */
void MainWindow::setTabStopWidth(int width)
{
    tabStopWidth = width;
    QFontMetrics metrics(font);
    ui->textEdit->setTabStopWidth(tabStopWidth * metrics.width(' '));
}


/* Sets the editor's font using the specified parameters.
 * @param family - the name of the font family
 * @param styleHint - used to select an appropriate default font family if the specified one is unavailable.
 * @param fixedPitch - if true, monospace font (equal-width characters)
 * @param pointSize - the size, in points, of the desired font (e.g., 12 for 12-pt font)
 */
void MainWindow::setFont(QString family, QFont::StyleHint styleHint,
                       bool fixedPitch, int pointSize)
{
    font.setFamily(family);
    font.setStyleHint(styleHint);
    font.setFixedPitch(fixedPitch);
    font.setPointSize(pointSize);
    ui->textEdit->setFont(font);
}


/* Returns the actual name of the file that's part of the given path.
 * @param filePath - the forward-slash-delimited path of the file
 */
QString MainWindow::getFileNameFromPath(QString filePath)
{
    if(filePath.isEmpty())
    {
        return defaultWindowTitle;
    }
    int indexOfLastForwardSlash = filePath.lastIndexOf('/');
    QString fileName = filePath.mid(indexOfLastForwardSlash + 1, filePath.length() - indexOfLastForwardSlash);
    return fileName;
}


/* Called when the user selects the new file creation option from the menu or toolbar.
 * If the current document has unsaved changes, it prompts the user to save or discard.
 * In either case, it ends up clearing the current file name and editor contents.
 */
void MainWindow::on_actionNew_triggered()
{
    if(fileNeedsToBeSaved)
    {
        QMessageBox::StandardButton userSelection;
        userSelection = promptYesOrNo("", "Do you want to save the changes to " + getFileNameFromPath(currentFile) + "?");

        if(userSelection == QMessageBox::Yes)
        {
            on_actionSave_triggered();
        }
    }

    resetEditor();
}


/* Called when the user selects the save option from the menu or toolbar.
 * If the current document is unnamed/unsaved, it prompts the user to specify
 * a name and directory in which to save the file. Otherwise, it overwrites
 * the contents of the current file on the disk.
 */
void MainWindow::on_actionSave_triggered()
{
    QString filePath;

    if(currentFile.isEmpty())
    {
        filePath = QFileDialog::getSaveFileName(this, "Save");
        currentFile = filePath;
    }
    else
    {
        filePath = currentFile;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    setWindowTitle(getFileNameFromPath(filePath));
    QTextStream out(&file);

    QString editorContents = ui->textEdit->toPlainText();
    out << editorContents;

    file.close();
    fileNeedsToBeSaved = false;
}


/* Called when the user selects the open option from the menu or toolbar.
 * Launches a dialog box that allows the user to select the file they want
 * to open. Sets the editor's current file path to that of the opened file on
 * success.
 */
void MainWindow::on_actionOpen_triggered()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Open");
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "Warning", "Cannot save file: " + file.errorString());
        return;
    }

    setWindowTitle(getFileNameFromPath(filePath));
    currentFile = filePath;

    QTextStream in(&file);
    QString documentContents = in.readAll();

    ui->textEdit->setText(documentContents);
    file.close();
    fileNeedsToBeSaved = false;
}


/* Called whenever the contents of the text editor change, even if they are deleted
 * and restored to their original state. Sets a flag that the current file needs to
 * be saved before any file creation or opening operations.
 */
void MainWindow::on_textEdit_textChanged() { fileNeedsToBeSaved = true; }
