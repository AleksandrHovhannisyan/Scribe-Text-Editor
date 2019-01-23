#ifndef TABBEDEDITOR_H
#define TABBEDEDITOR_H
#include <QTabWidget>
#include <editor.h>

class TabbedEditor : public QTabWidget
{
    Q_OBJECT

public:
    TabbedEditor(QWidget *parent = nullptr);
    void add(Editor* tab);

protected:
    bool eventFilter(QObject* obj, QEvent* event) override;
};

#endif // TABBEDEDITOR_H
