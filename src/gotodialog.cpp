#include "gotodialog.h"
#include <QMessageBox>


/* Initializes this GotoDialog object.
 */
GotoDialog::GotoDialog(QWidget *parent) : QDialog(parent)
{
    gotoLabel = new QLabel(tr("Line: "));
    gotoLineEdit = new QLineEdit();
    gotoButton = new QPushButton("Go");
    layout = new QHBoxLayout();

    layout->addWidget(gotoLabel);
    layout->addWidget(gotoLineEdit);
    layout->addWidget(gotoButton);

    setLayout(layout);
    setWindowTitle(tr("Go To"));

    // Ensures the line edit gets the focus when the dialog is launched
    setFocusProxy(gotoLineEdit);

    connect(gotoButton, SIGNAL(clicked()), this, SLOT(on_gotoButton_clicked()));
}


/* Performs all necessary memory cleanup operations.
 */
GotoDialog::~GotoDialog()
{
    delete gotoLabel;
    delete gotoLineEdit;
    delete gotoButton;
    delete layout;
}


/* Called when the user clicks the Go button.
 */
void GotoDialog::on_gotoButton_clicked()
{
    QString line = gotoLineEdit->text();

    if (line.isEmpty())
    {
        QMessageBox::information(this, tr("Go"), tr("Must enter a line number."));
        return;
    }

    emit(gotoLine(line.toInt()));
}
