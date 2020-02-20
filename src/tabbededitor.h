#ifndef TABBEDEDITOR_H
#define TABBEDEDITOR_H
#include <QTabWidget>
#include <editor.h>
#include <QVector>

class TabbedEditor : public QTabWidget
{
    Q_OBJECT

public:

    TabbedEditor(QWidget *parent = nullptr);
    void add(Editor* tab);

    Editor *currentTab() const;
    Editor *tabAt(int index) const;
    QVector<Editor*> tabs() const;
    int numTabs() const { return tabs().length(); }
    QVector<Editor*> unsavedTabs() const;

    void promptFontSelection();
    bool applyWordWrapping(bool shouldWrap);
    bool applyAutoIndentation(bool shouldAutoIndent);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};

#endif // TABBEDEDITOR_H
