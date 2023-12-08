/********************************************************************************
** Form generated from reading UI file 'deviceinputdialog.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DEVICEINPUTDIALOG_H
#define UI_DEVICEINPUTDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>

QT_BEGIN_NAMESPACE

class Ui_DeviceInputDialog
{
public:
    QDialogButtonBox *buttonBox;
    QLineEdit *lineEdit;
    QLabel *label;

    void setupUi(QDialog *DeviceInputDialog)
    {
        if (DeviceInputDialog->objectName().isEmpty())
            DeviceInputDialog->setObjectName(QString::fromUtf8("DeviceInputDialog"));
        buttonBox = new QDialogButtonBox(DeviceInputDialog);
        buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
        buttonBox->setGeometry(QRect(100, 130, 341, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        lineEdit = new QLineEdit(DeviceInputDialog);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setGeometry(QRect(20, 70, 421, 41));
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(lineEdit->sizePolicy().hasHeightForWidth());
        lineEdit->setSizePolicy(sizePolicy);
        QFont font;
        font.setPointSize(14);
        lineEdit->setFont(font);
        label = new QLabel(DeviceInputDialog);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(30, 20, 411, 41));
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        QFont font1;
        font1.setPointSize(18);
        label->setFont(font1);

        retranslateUi(DeviceInputDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), DeviceInputDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), DeviceInputDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(DeviceInputDialog);
    } // setupUi

    void retranslateUi(QDialog *DeviceInputDialog)
    {
        DeviceInputDialog->setWindowTitle(QCoreApplication::translate("DeviceInputDialog", "What Device To Use?", nullptr));
        label->setText(QCoreApplication::translate("DeviceInputDialog", "Device Name:", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DeviceInputDialog: public Ui_DeviceInputDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DEVICEINPUTDIALOG_H
