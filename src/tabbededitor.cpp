#include "tabbededitor.h"
#include "utilityfunctions.h"
#include <QFont>
#include <QFontDialog>
#include <QtDebug>


/* Initializes this TabbedEditor with a single Editor tab.
 */
TabbedEditor::TabbedEditor(QWidget *parent) : QTabWidget(parent)
{
    add(new Editor());
    installEventFilter(this);
    setMovable(true);
}


/* Adds the given Editor as a new tab.
 * @tab - the Editor object to add as a tab of this widget
 */
void TabbedEditor::add(Editor *tab)
{
    QTabWidget::addTab(tab, tab->getFileName());
    setCurrentWidget(tab);
}


/* Returns a pointer to the current Editor tab of this TabbedEditor.
 */
Editor* TabbedEditor::currentTab() const
{
    return qobject_cast<Editor*>(widget(currentIndex()));
}


/* Returns the tab at the specified index (0 to count() -1).
 */
Editor* TabbedEditor::tabAt(int index) const
{
    if (index < 0 || index >= count())
    {
        return nullptr;
    }

    return qobject_cast<Editor*>(widget(index));
}


/* Returns a vector of all Editor tabs this TabbedEditor contains.
 */
QVector<Editor*> TabbedEditor::tabs() const
{
    QVector<Editor*> tabs;

    for (int i = 0; i < count(); i++)
    {
        tabs.push_back(tabAt(i));
    }

    return tabs;
}


/* Returns a vector of all Editor tabs that are unsaved.
 */
QVector<Editor*> TabbedEditor::unsavedTabs() const
{
    QVector<Editor*> unsavedTabs;

    for (int i = 0; i < count(); i++)
    {
        Editor *tab = tabAt(i);

        if (tab->isUnsaved())
        {
            unsavedTabs.push_back(tab);
        }
    }

    return unsavedTabs;
}


/* Launches a QFontDialog to allow the user to select a font.
 */
void TabbedEditor::promptFontSelection()
{
    bool userChoseFont;
    QFont newFont = QFontDialog::getFont(&userChoseFont, currentTab()->getFont(), this);

    if (!userChoseFont) return;

    QMessageBox::StandardButton tabSelection = Utility::promptYesOrNo(this, tr("Font change"),
                                                                      tr("Apply to all open and future tabs?"));

    // Apply font to all tabs
    if (tabSelection == QMessageBox::Yes)
    {
        for (Editor *tab : tabs())
        {
            tab->setFont(newFont, QFont::Monospace, true, Editor::NUM_CHARS_FOR_TAB);
        }
    }

    // Apply font only to current tab
    else if (tabSelection == QMessageBox::No)
    {
        currentTab()->setFont(newFont, QFont::Monospace, true, Editor::NUM_CHARS_FOR_TAB);
    }

    // If user hit cancel
    else
    {
        return;
    }
}


/* Applies word wrapping to either all tabs or the current tab, depending on the user's selection.
 */
bool TabbedEditor::applyWordWrapping(bool shouldWrap)
{
    if (numTabs() == 1)
    {
        currentTab()->toggleWrapMode(shouldWrap);
        return true;
    }

    // Anything below only applies if there are > 1 tabs open
    QMessageBox::StandardButton tabSelection = Utility::promptYesOrNo(this, tr("Word wrapping"),
                                                                      tr("Apply to all tabs?"));

    // Apply wrapping to all tabs
    if (tabSelection == QMessageBox::Yes)
    {
        for (Editor *tab : tabs())
        {
            tab->toggleWrapMode(shouldWrap);
        }

        return true;
    }

    // Apply wrapping only to current tab
    else if (tabSelection == QMessageBox::No)
    {
        currentTab()->toggleWrapMode(shouldWrap);
        return true;
    }

    // If user hit cancel
    else
    {
        return false;
    }
}


/* Applies auto indentation to either all tabs or the current tab, depending on the user's selection.
 * Returns true if the formatting was applied at all, and false if the operation was canceled.
 */
bool TabbedEditor::applyAutoIndentation(bool shouldAutoIndent)
{
    if (numTabs() == 1)
    {
        currentTab()->toggleAutoIndent(shouldAutoIndent);
        return true;
    }

    // Anything below only applies if there are > 1 tabs open
    QMessageBox::StandardButton tabSelection = Utility::promptYesOrNo(this, tr("Auto indentation"),
                                                                      tr("Apply to all open and future tabs?"));

    // Apply auto indentation to all tabs
    if (tabSelection == QMessageBox::Yes)
    {
        for (Editor *tab : tabs())
        {
            tab->toggleAutoIndent(shouldAutoIndent);
        }

        return true;
    }

    // Apply auto indentation only to current tab
    else if (tabSelection == QMessageBox::No)
    {
        currentTab()->toggleAutoIndent(shouldAutoIndent);
        return true;
    }

    // If user hit cancel
    else
    {
        return false;
    }
}


/* Handles input events. Mainly used to allow the user to switch tabs with ctrl + num
 * and ctrl + tab.
 */
bool TabbedEditor::eventFilter(QObject* obj, QEvent* event)
{
    bool isKeyPress = event->type() == QEvent::KeyPress;

    if (isKeyPress)
    {
        QKeyEvent *keyInfo = static_cast<QKeyEvent*>(event);
        int key = keyInfo->key();

        if (keyInfo->modifiers() == Qt::ControlModifier)
        {
            // Ctrl + num = jump to that tab number
            if (key >= Qt::Key_1 && key <= Qt::Key_9)
            {
                setCurrentWidget(tabAt(key - Qt::Key_1));
                return true;
            }

            // Ctrl + tab = advance tab by one
            else if (key == Qt::Key_T)
            {
                int newTabIndex = (currentIndex() + 1) % count();
                setCurrentWidget(tabAt(newTabIndex));
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
