#include "tabbededitor.h"


/* Initializes this TabbedEditor with a single Editor tab.
 */
TabbedEditor::TabbedEditor(QWidget *parent) : QTabWidget(parent)
{
    add(new Editor());
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
