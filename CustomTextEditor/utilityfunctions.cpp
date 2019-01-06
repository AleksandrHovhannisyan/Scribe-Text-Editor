#include "utilityfunctions.h"

/* Launches a Yes or No message box within the context of the given
 * parent widget. Prompts the user to make a selection.
 */
QMessageBox::StandardButton Utility::promptYesOrNo(QWidget *parent, QString title, QString prompt)
{
    return QMessageBox::question(parent, title, prompt, QMessageBox::Yes | QMessageBox::No);
}

