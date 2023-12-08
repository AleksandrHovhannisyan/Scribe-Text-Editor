#include "memoryterminal.h"
#include "ui_memoryterminal.h"

MemoryTerminal::MemoryTerminal(QWidget *parent, QProcess *proc) :
    QWidget(parent),
    ui(new Ui::MemoryTerminal),
    proc(proc)
{

    ui->setupUi(this);
    connect(ui->killMemory, SIGNAL(clicked()), this, SLOT(killMemory()));
}

MemoryTerminal::~MemoryTerminal()
{
    delete ui;
}

void MemoryTerminal::killMemory()
{
    proc->kill();
    this->close();
}

void MemoryTerminal::reportCurrentProcess()
{
    QByteArray info = proc->readAllStandardOutput();
    ui->MemoryActionOutput->appendPlainText(info);
}
