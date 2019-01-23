#include "tabbededitor.h"
#include <QtDebug>

/* Initializes this TabbedEditor with a single Editor tab.
 */
TabbedEditor::TabbedEditor(QWidget *parent) : QTabWidget(parent)
{
    add(new Editor());
    installEventFilter(this);
}


/* Adds the given Editor as a new tab.
 * @tab - the Editor object to add as a tab of this widget
 */
void TabbedEditor::add(Editor *tab)
{
    QTabWidget::addTab(tab, tab->getFileName());
    tab->setFont("Courier", QFont::Monospace, true, 10, 5);
    setCurrentWidget(tab);
}


/* Handles input events. Mainly used to allow the user to switch tabs with ctrl + num
 * and ctrl + tab.
 */
bool TabbedEditor::eventFilter(QObject* obj, QEvent* event)
{
    bool isKeyPress = event->type() == QEvent::KeyPress;

    if(isKeyPress)
    {
        QKeyEvent *keyInfo = static_cast<QKeyEvent*>(event);
        int key = keyInfo->key();

        if(keyInfo->modifiers() == Qt::ControlModifier)
        {
            // Ctrl + num = jump to that tab number
            if(key >= Qt::Key_1 && key <= Qt::Key_9)
            {
                setCurrentWidget(widget(key - Qt::Key_1));
                return true;
            }

            // Ctrl + tab = advance tab by one
            else if(key == Qt::Key_T)
            {
                int newTabIndex = (currentIndex() + 1) % count();
                setCurrentWidget(widget(newTabIndex));
                return true;
            }
        }
        else
        {
            return QObject::eventFilter(obj, event);
        }
    }

    return QObject::eventFilter(obj, event);
}
