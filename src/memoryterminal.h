#ifndef MEMORYTERMINAL_H
#define MEMORYTERMINAL_H

#include "qprocess.h"
#include <QWidget>

namespace Ui {
class MemoryTerminal;
}

class MemoryTerminal : public QWidget
{
    Q_OBJECT

public:
    explicit MemoryTerminal(QWidget *parent = nullptr, QProcess *proc = nullptr);
    ~MemoryTerminal();
public slots:
    void killMemory();
    void reportCurrentProcess();

private:
    Ui::MemoryTerminal *ui;
    QProcess *proc;
};

#endif // MEMORYTERMINAL_H
