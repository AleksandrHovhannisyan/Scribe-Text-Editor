#include "utilityfunctions.h"
#include <QStack>
#include <QtDebug>

/* Launches a Yes or No message box within the context of the given
 * parent widget. Prompts the user to make a selection.
 */
QMessageBox::StandardButton Utility::promptYesOrNo(QWidget *parent, QString title, QString prompt)
{
    return QMessageBox::question(parent, title, prompt, QMessageBox::Yes | QMessageBox::No);
}


/* Returns true if the curly braces in the given string are balanced
 * and false otherwise.
 */
bool Utility::curlyBracesMatch(QString context, int startIndex)
{
    QStack<char> expectedClosingBraces;
    expectedClosingBraces.push('t');

    for(int i = startIndex; i < context.length(); i++)
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
        }
    }

    qDebug() << "Remaining expected closing braces: " << expectedClosingBraces.size();

    return expectedClosingBraces.empty();
}
