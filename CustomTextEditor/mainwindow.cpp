#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionNew_triggered()
{
    bool editorIsEmpty = ui->textEdit->toPlainText().isEmpty();

    // TODO handle case where the user tries to create a new file while there are unsaved contents
    if(!editorIsEmpty)
    {
    }
}

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

    setWindowTitle(filePath);
    QTextStream out(&file);

    QString editorContents = ui->textEdit->toPlainText();
    out << editorContents;

    file.close();

}
