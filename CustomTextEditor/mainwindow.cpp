#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(defaultWindowTitle);
}

MainWindow::~MainWindow()
{
    delete ui;
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
    bool editorNotEmpty = !ui->textEdit->toPlainText().isEmpty();

    if(editorNotEmpty && fileNeedsToBeSaved)
    {
        QMessageBox::StandardButton userSelection;
        userSelection = QMessageBox::question(this, "Text Editor",
                              "Do you want to save the changes to " + getFileNameFromPath(currentFile) + "?",
                              QMessageBox::Yes | QMessageBox::No);

        if(userSelection == QMessageBox::Yes)
        {
            on_actionSave_triggered();
        }
    }

    currentFile.clear();
    ui->textEdit->setText(QString());
    setWindowTitle(defaultWindowTitle);
}

/* TODO document
 *
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

/* TODO document
 *
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

/* TODO document
 *
 */
void MainWindow::on_textEdit_textChanged() { fileNeedsToBeSaved = true; }
