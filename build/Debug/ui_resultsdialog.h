/********************************************************************************
** Form generated from reading UI file 'resultsdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RESULTSDIALOG_H
#define UI_RESULTSDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>

QT_BEGIN_NAMESPACE

class Ui_ResultsDialog
{
public:
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ResultsDialog)
    {
        if (ResultsDialog->objectName().isEmpty())
            ResultsDialog->setObjectName(QString::fromUtf8("ResultsDialog"));
        ResultsDialog->resize(400, 300);
        buttonBox = new QDialogButtonBox(ResultsDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(30, 240, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        retranslateUi(ResultsDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), ResultsDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ResultsDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(ResultsDialog);
    } // setupUi

    void retranslateUi(QDialog *ResultsDialog)
    {
        ResultsDialog->setWindowTitle(QCoreApplication::translate("ResultsDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ResultsDialog: public Ui_ResultsDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RESULTSDIALOG_H
