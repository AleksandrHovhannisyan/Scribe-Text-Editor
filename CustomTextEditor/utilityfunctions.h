#ifndef UTILITYFUNCTIONS_H
#define UTILITYFUNCTIONS_H
#include <QString>
#include <QMessageBox>

class Utility {
public:
    static QMessageBox::StandardButton promptYesOrNo(QWidget *parent, QString title, QString prompt);
    static bool hasBalancedCurlyBraces(QString context);
};


#endif // UTILITYFUNCTIONS_H
