#include "utilityfunctions.h"
#include <QStack>
#include <QtDebug>

/* Launches a Yes or No message box within the context of the given
 * parent widget. Prompts the user to make a selection.
 */
QMessageBox::StandardButton Utility::promptYesOrNo(QWidget *parent, QString title, QString prompt)
{
    QMessageBox asker;
    asker.setEscapeButton(QMessageBox::StandardButton::Cancel);
    return asker.question(parent, title, prompt, QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel, QMessageBox::Yes);
}


/* Returns true if the curly braces in the given string are balanced
 * and false otherwise.
 */
bool Utility::hasBalancedCurlyBraces(QString context)
{
    QStack<char> expectedClosingBraces;

    for(int i = 0; i < context.length(); i++)
    {
        char character = context.at(i).toLatin1();

        if(character == '{')
        {
            expectedClosingBraces.push('{');
        }

        else if(character == '}')
        {
            if(!expectedClosingBraces.empty())
            {
                expectedClosingBraces.pop();
            }
            else
            {
                return false;
            }
        }
    }

    return expectedClosingBraces.empty();
}
