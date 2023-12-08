/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "tabbededitor.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionNew;
    QAction *actionOpen;
    QAction *actionSave;
    QAction *actionSave_As;
    QAction *actionPrint;
    QAction *actionExit;
    QAction *actionUndo;
    QAction *actionCut;
    QAction *actionCopy;
    QAction *actionPaste;
    QAction *actionFind;
    QAction *actionReplace;
    QAction *actionGo_To;
    QAction *actionSelect_All;
    QAction *actionTime_Date;
    QAction *actionFont;
    QAction *actionStatus_Bar;
    QAction *actionRedo;
    QAction *actionAuto_Indent;
    QAction *actionWord_Wrap;
    QAction *actionC_Lang;
    QAction *actionCPP_Lang;
    QAction *actionJava_Lang;
    QAction *actionPython_Lang;
    QAction *actionTool_Bar;
    QAction *actionCompileRiscV;
    QAction *actionUploadToSCMB;
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    TabbedEditor *tabWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuFormat;
    QMenu *menuLanguage;
    QMenu *menuView;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(870, 420);
        MainWindow->setMinimumSize(QSize(530, 230));
        actionNew = new QAction(MainWindow);
        actionNew->setObjectName(QString::fromUtf8("actionNew"));
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/icons/res/icons/new.bmp"), QSize(), QIcon::Normal, QIcon::On);
        actionNew->setIcon(icon);
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName(QString::fromUtf8("actionOpen"));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/icons/res/icons/folder.bmp"), QSize(), QIcon::Normal, QIcon::On);
        actionOpen->setIcon(icon1);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName(QString::fromUtf8("actionSave"));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/icons/res/icons/save.bmp"), QSize(), QIcon::Normal, QIcon::On);
        actionSave->setIcon(icon2);
        actionSave_As = new QAction(MainWindow);
        actionSave_As->setObjectName(QString::fromUtf8("actionSave_As"));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/icons/res/icons/save-as.bmp"), QSize(), QIcon::Normal, QIcon::On);
        actionSave_As->setIcon(icon3);
        actionPrint = new QAction(MainWindow);
        actionPrint->setObjectName(QString::fromUtf8("actionPrint"));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/icons/res/icons/printer.bmp"), QSize(), QIcon::Normal, QIcon::On);
        actionPrint->setIcon(icon4);
        actionExit = new QAction(MainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        actionUndo = new QAction(MainWindow);
        actionUndo->setObjectName(QString::fromUtf8("actionUndo"));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/icons/res/icons/undo.bmp"), QSize(), QIcon::Normal, QIcon::On);
        actionUndo->setIcon(icon5);
        actionCut = new QAction(MainWindow);
        actionCut->setObjectName(QString::fromUtf8("actionCut"));
        QIcon icon6;
        icon6.addFile(QString::fromUtf8(":/icons/res/icons/cut.bmp"), QSize(), QIcon::Normal, QIcon::On);
        actionCut->setIcon(icon6);
        actionCopy = new QAction(MainWindow);
        actionCopy->setObjectName(QString::fromUtf8("actionCopy"));
        QIcon icon7;
        icon7.addFile(QString::fromUtf8(":/icons/res/icons/copy.bmp"), QSize(), QIcon::Normal, QIcon::On);
        actionCopy->setIcon(icon7);
        actionPaste = new QAction(MainWindow);
        actionPaste->setObjectName(QString::fromUtf8("actionPaste"));
        QIcon icon8;
        icon8.addFile(QString::fromUtf8(":/icons/res/icons/paste.bmp"), QSize(), QIcon::Normal, QIcon::On);
        actionPaste->setIcon(icon8);
        actionFind = new QAction(MainWindow);
        actionFind->setObjectName(QString::fromUtf8("actionFind"));
        QIcon icon9;
        icon9.addFile(QString::fromUtf8(":/icons/res/icons/search.bmp"), QSize(), QIcon::Normal, QIcon::On);
        actionFind->setIcon(icon9);
        actionReplace = new QAction(MainWindow);
        actionReplace->setObjectName(QString::fromUtf8("actionReplace"));
        actionGo_To = new QAction(MainWindow);
        actionGo_To->setObjectName(QString::fromUtf8("actionGo_To"));
        actionSelect_All = new QAction(MainWindow);
        actionSelect_All->setObjectName(QString::fromUtf8("actionSelect_All"));
        actionTime_Date = new QAction(MainWindow);
        actionTime_Date->setObjectName(QString::fromUtf8("actionTime_Date"));
        actionFont = new QAction(MainWindow);
        actionFont->setObjectName(QString::fromUtf8("actionFont"));
        actionStatus_Bar = new QAction(MainWindow);
        actionStatus_Bar->setObjectName(QString::fromUtf8("actionStatus_Bar"));
        actionStatus_Bar->setCheckable(true);
        actionStatus_Bar->setChecked(true);
        actionStatus_Bar->setEnabled(true);
        actionRedo = new QAction(MainWindow);
        actionRedo->setObjectName(QString::fromUtf8("actionRedo"));
        QIcon icon10;
        icon10.addFile(QString::fromUtf8(":/icons/res/icons/redo.bmp"), QSize(), QIcon::Normal, QIcon::On);
        actionRedo->setIcon(icon10);
        actionAuto_Indent = new QAction(MainWindow);
        actionAuto_Indent->setObjectName(QString::fromUtf8("actionAuto_Indent"));
        actionAuto_Indent->setCheckable(true);
        actionAuto_Indent->setChecked(true);
        actionWord_Wrap = new QAction(MainWindow);
        actionWord_Wrap->setObjectName(QString::fromUtf8("actionWord_Wrap"));
        actionWord_Wrap->setCheckable(true);
        actionC_Lang = new QAction(MainWindow);
        actionC_Lang->setObjectName(QString::fromUtf8("actionC_Lang"));
        actionC_Lang->setCheckable(true);
        actionCPP_Lang = new QAction(MainWindow);
        actionCPP_Lang->setObjectName(QString::fromUtf8("actionCPP_Lang"));
        actionCPP_Lang->setCheckable(true);
        actionJava_Lang = new QAction(MainWindow);
        actionJava_Lang->setObjectName(QString::fromUtf8("actionJava_Lang"));
        actionJava_Lang->setCheckable(true);
        actionPython_Lang = new QAction(MainWindow);
        actionPython_Lang->setObjectName(QString::fromUtf8("actionPython_Lang"));
        actionPython_Lang->setCheckable(true);
        actionTool_Bar = new QAction(MainWindow);
        actionTool_Bar->setObjectName(QString::fromUtf8("actionTool_Bar"));
        actionTool_Bar->setCheckable(true);
        actionTool_Bar->setChecked(true);
        actionCompileRiscV = new QAction(MainWindow);
        actionCompileRiscV->setObjectName(QString::fromUtf8("actionCompileRiscV"));
        QIcon icon11;
        icon11.addFile(QString::fromUtf8(":/icons/res/icons/compfile-256.png"), QSize(), QIcon::Normal, QIcon::On);
        actionCompileRiscV->setIcon(icon11);
        actionUploadToSCMB = new QAction(MainWindow);
        actionUploadToSCMB->setObjectName(QString::fromUtf8("actionUploadToSCMB"));
        QIcon icon12;
        icon12.addFile(QString::fromUtf8(":/icons/res/icons/upload.png"), QSize(), QIcon::Normal, QIcon::Off);
        icon12.addFile(QString::fromUtf8("res/icons/upload.png"), QSize(), QIcon::Normal, QIcon::On);
        actionUploadToSCMB->setIcon(icon12);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        tabWidget = new TabbedEditor(centralWidget);
        tabWidget->setObjectName(QString::fromUtf8("tabWidget"));

        verticalLayout->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 870, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QString::fromUtf8("menuFile"));
        menuEdit = new QMenu(menuBar);
        menuEdit->setObjectName(QString::fromUtf8("menuEdit"));
        menuFormat = new QMenu(menuBar);
        menuFormat->setObjectName(QString::fromUtf8("menuFormat"));
        menuLanguage = new QMenu(menuFormat);
        menuLanguage->setObjectName(QString::fromUtf8("menuLanguage"));
        menuView = new QMenu(menuBar);
        menuView->setObjectName(QString::fromUtf8("menuView"));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        statusBar->setEnabled(true);
        MainWindow->setStatusBar(statusBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuEdit->menuAction());
        menuBar->addAction(menuFormat->menuAction());
        menuBar->addAction(menuView->menuAction());
        menuFile->addAction(actionNew);
        menuFile->addAction(actionOpen);
        menuFile->addAction(actionSave);
        menuFile->addAction(actionSave_As);
        menuFile->addSeparator();
        menuFile->addAction(actionPrint);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuEdit->addAction(actionUndo);
        menuEdit->addAction(actionRedo);
        menuEdit->addSeparator();
        menuEdit->addAction(actionCut);
        menuEdit->addAction(actionCopy);
        menuEdit->addAction(actionPaste);
        menuEdit->addSeparator();
        menuEdit->addAction(actionFind);
        menuEdit->addAction(actionReplace);
        menuEdit->addAction(actionGo_To);
        menuEdit->addSeparator();
        menuEdit->addAction(actionSelect_All);
        menuEdit->addAction(actionTime_Date);
        menuFormat->addAction(actionFont);
        menuFormat->addAction(menuLanguage->menuAction());
        menuFormat->addAction(actionAuto_Indent);
        menuFormat->addAction(actionWord_Wrap);
        menuLanguage->addAction(actionC_Lang);
        menuLanguage->addAction(actionCPP_Lang);
        menuLanguage->addAction(actionJava_Lang);
        menuLanguage->addAction(actionPython_Lang);
        menuView->addAction(actionStatus_Bar);
        menuView->addAction(actionTool_Bar);
        mainToolBar->addAction(actionNew);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addAction(actionSave);
        mainToolBar->addAction(actionSave_As);
        mainToolBar->addAction(actionPrint);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionUndo);
        mainToolBar->addAction(actionRedo);
        mainToolBar->addAction(actionCopy);
        mainToolBar->addAction(actionCut);
        mainToolBar->addAction(actionPaste);
        mainToolBar->addAction(actionFind);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionCompileRiscV);
        mainToolBar->addAction(actionUploadToSCMB);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionNew->setText(QCoreApplication::translate("MainWindow", "New", nullptr));
#if QT_CONFIG(shortcut)
        actionNew->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+N", nullptr));
#endif // QT_CONFIG(shortcut)
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Open", nullptr));
#if QT_CONFIG(shortcut)
        actionOpen->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+O", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave->setText(QCoreApplication::translate("MainWindow", "Save", nullptr));
#if QT_CONFIG(shortcut)
        actionSave->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSave_As->setText(QCoreApplication::translate("MainWindow", "Save As", nullptr));
#if QT_CONFIG(shortcut)
        actionSave_As->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Shift+S", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPrint->setText(QCoreApplication::translate("MainWindow", "Print...", nullptr));
#if QT_CONFIG(shortcut)
        actionPrint->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+P", nullptr));
#endif // QT_CONFIG(shortcut)
        actionExit->setText(QCoreApplication::translate("MainWindow", "Exit", nullptr));
        actionUndo->setText(QCoreApplication::translate("MainWindow", "Undo", nullptr));
#if QT_CONFIG(shortcut)
        actionUndo->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Z", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCut->setText(QCoreApplication::translate("MainWindow", "Cut", nullptr));
#if QT_CONFIG(shortcut)
        actionCut->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+X", nullptr));
#endif // QT_CONFIG(shortcut)
        actionCopy->setText(QCoreApplication::translate("MainWindow", "Copy", nullptr));
#if QT_CONFIG(shortcut)
        actionCopy->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+C", nullptr));
#endif // QT_CONFIG(shortcut)
        actionPaste->setText(QCoreApplication::translate("MainWindow", "Paste", nullptr));
#if QT_CONFIG(shortcut)
        actionPaste->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+V", nullptr));
#endif // QT_CONFIG(shortcut)
        actionFind->setText(QCoreApplication::translate("MainWindow", "Find...", nullptr));
#if QT_CONFIG(shortcut)
        actionFind->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+F", nullptr));
#endif // QT_CONFIG(shortcut)
        actionReplace->setText(QCoreApplication::translate("MainWindow", "Replace...", nullptr));
#if QT_CONFIG(shortcut)
        actionReplace->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+H", nullptr));
#endif // QT_CONFIG(shortcut)
        actionGo_To->setText(QCoreApplication::translate("MainWindow", "Go To...", nullptr));
#if QT_CONFIG(shortcut)
        actionGo_To->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+G", nullptr));
#endif // QT_CONFIG(shortcut)
        actionSelect_All->setText(QCoreApplication::translate("MainWindow", "Select All", nullptr));
#if QT_CONFIG(shortcut)
        actionSelect_All->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+A", nullptr));
#endif // QT_CONFIG(shortcut)
        actionTime_Date->setText(QCoreApplication::translate("MainWindow", "Time/Date", nullptr));
#if QT_CONFIG(shortcut)
        actionTime_Date->setShortcut(QCoreApplication::translate("MainWindow", "F5", nullptr));
#endif // QT_CONFIG(shortcut)
        actionFont->setText(QCoreApplication::translate("MainWindow", "Font...", nullptr));
        actionStatus_Bar->setText(QCoreApplication::translate("MainWindow", "Status Bar", nullptr));
        actionRedo->setText(QCoreApplication::translate("MainWindow", "Redo", nullptr));
#if QT_CONFIG(shortcut)
        actionRedo->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+Y", nullptr));
#endif // QT_CONFIG(shortcut)
        actionAuto_Indent->setText(QCoreApplication::translate("MainWindow", "Auto Indent", nullptr));
        actionWord_Wrap->setText(QCoreApplication::translate("MainWindow", "Word Wrap", nullptr));
        actionC_Lang->setText(QCoreApplication::translate("MainWindow", "C", nullptr));
        actionCPP_Lang->setText(QCoreApplication::translate("MainWindow", "C++", nullptr));
        actionJava_Lang->setText(QCoreApplication::translate("MainWindow", "Java", nullptr));
        actionPython_Lang->setText(QCoreApplication::translate("MainWindow", "Python", nullptr));
        actionTool_Bar->setText(QCoreApplication::translate("MainWindow", "Tool Bar", nullptr));
        actionCompileRiscV->setText(QCoreApplication::translate("MainWindow", "CompileRiscV", nullptr));
#if QT_CONFIG(shortcut)
        actionCompileRiscV->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+B", nullptr));
#endif // QT_CONFIG(shortcut)
        actionUploadToSCMB->setText(QCoreApplication::translate("MainWindow", "UploadToSCMB", nullptr));
#if QT_CONFIG(tooltip)
        actionUploadToSCMB->setToolTip(QCoreApplication::translate("MainWindow", "Upload to SCMB Board", nullptr));
#endif // QT_CONFIG(tooltip)
#if QT_CONFIG(shortcut)
        actionUploadToSCMB->setShortcut(QCoreApplication::translate("MainWindow", "Ctrl+M", nullptr));
#endif // QT_CONFIG(shortcut)
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuEdit->setTitle(QCoreApplication::translate("MainWindow", "Edit", nullptr));
        menuFormat->setTitle(QCoreApplication::translate("MainWindow", "Format", nullptr));
        menuLanguage->setTitle(QCoreApplication::translate("MainWindow", "Language", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "View", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
