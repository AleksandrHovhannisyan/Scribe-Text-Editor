#include "utilityfunctions.h"
#include <QStack>
#include <QtDebug>
#include <QQueue>


/* Launches a Yes or No message box within the context of the given
 * parent widget. Prompts the user to make a selection.
 */
QMessageBox::StandardButton Utility::promptYesOrNo(QWidget *parent, QString title, QString prompt)
{
    QMessageBox asker;
    asker.setEscapeButton(QMessageBox::StandardButton::Cancel);
    return asker.question(parent, title, prompt, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
}


/* Returns true if a closing brace must be inserted into the given string
 * to create a balanced expression and false otherwise.
 */
bool Utility::codeBlockNotClosed(QString context, QChar startDelimiter, QChar endDelimiter)
{
    QStack<char> codeBlockStartDelimiters;

    for (int i = 0; i < context.length(); i++)
    {
        char character = context.at(i).toLatin1();

        if (character == startDelimiter)
        {
            codeBlockStartDelimiters.push(character);
        }

        else if (character == endDelimiter && !codeBlockStartDelimiters.empty())
        {
            codeBlockStartDelimiters.pop();
        }
    }

    return !codeBlockStartDelimiters.empty();
}
