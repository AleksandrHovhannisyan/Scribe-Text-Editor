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


/* Returns true if the opening brace at the given index is balanced and false otherwise.
 */
bool Utility::braceIsBalanced(QString context, int openBraceIndex)
{
    QQueue<int> openBraceIndices;
    QStack<char> expectedClosingBraces;

    for(int i = 0; i < context.length(); i++)
    {
        char character = context.at(i).toLatin1();

        if(character == '{')
        {
            openBraceIndices.enqueue(i);
            expectedClosingBraces.push('}');
        }
        else if(character == '}')
        {
            if(!expectedClosingBraces.empty() && !openBraceIndices.empty())
            {
                expectedClosingBraces.pop();
                openBraceIndices.dequeue();
            }
        }
    }

    while(!openBraceIndices.empty())
    {
        int index = openBraceIndices.dequeue();

        if(index == openBraceIndex)
        {
            return false;
        }
    }

    return true;
}



/* Returns the index of the first unbalanced closing brace within the given context.
 */
int Utility::indexOfFirstUnbalancedClosingBrace(QString context)
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
                return i;
            }
        }
    }

    return -1;
}
