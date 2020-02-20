#ifndef UTILITYFUNCTIONS_H
#define UTILITYFUNCTIONS_H
#include <QString>
#include <QMessageBox>


namespace Utility
{
    QMessageBox::StandardButton promptYesOrNo(QWidget *parent, QString title, QString prompt);
    bool codeBlockNotClosed(QString context, QChar startDelimiter, QChar endDelimiter);
}

#endif // UTILITYFUNCTIONS_H
