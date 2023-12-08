/********************************************************************************
** Form generated from reading UI file 'memoryterminal.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MEMORYTERMINAL_H
#define UI_MEMORYTERMINAL_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MemoryTerminal
{
public:
    QAction *actionkillMemoryProcess;
    QPlainTextEdit *MemoryActionOutput;
    QPushButton *killMemory;

    void setupUi(QWidget *MemoryTerminal)
    {
        if (MemoryTerminal->objectName().isEmpty())
            MemoryTerminal->setObjectName(QString::fromUtf8("MemoryTerminal"));
        MemoryTerminal->resize(825, 419);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MemoryTerminal->sizePolicy().hasHeightForWidth());
        MemoryTerminal->setSizePolicy(sizePolicy);
        actionkillMemoryProcess = new QAction(MemoryTerminal);
        actionkillMemoryProcess->setObjectName(QString::fromUtf8("actionkillMemoryProcess"));
        MemoryActionOutput = new QPlainTextEdit(MemoryTerminal);
        MemoryActionOutput->setObjectName(QString::fromUtf8("MemoryActionOutput"));
        MemoryActionOutput->setGeometry(QRect(10, 70, 811, 341));
        MemoryActionOutput->setLineWidth(1);
        MemoryActionOutput->setReadOnly(true);
        killMemory = new QPushButton(MemoryTerminal);
        killMemory->setObjectName(QString::fromUtf8("killMemory"));
        killMemory->setGeometry(QRect(10, 0, 91, 61));
        QSizePolicy sizePolicy1(QSizePolicy::MinimumExpanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(killMemory->sizePolicy().hasHeightForWidth());
        killMemory->setSizePolicy(sizePolicy1);

        retranslateUi(MemoryTerminal);

        QMetaObject::connectSlotsByName(MemoryTerminal);
    } // setupUi

    void retranslateUi(QWidget *MemoryTerminal)
    {
        MemoryTerminal->setWindowTitle(QCoreApplication::translate("MemoryTerminal", "Form", nullptr));
        actionkillMemoryProcess->setText(QCoreApplication::translate("MemoryTerminal", "killMemoryProcess", nullptr));
        killMemory->setText(QCoreApplication::translate("MemoryTerminal", "Kill Memory", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MemoryTerminal: public Ui_MemoryTerminal {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MEMORYTERMINAL_H
