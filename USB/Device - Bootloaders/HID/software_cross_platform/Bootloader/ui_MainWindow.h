/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.0.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QAction *actionOpen;
    QAction *actionExit;
    QAction *actionWrite_Device;
    QAction *actionErase_Device;
    QAction *action_About;
    QAction *action_Verify_Device;
    QAction *action_Settings;
    QAction *actionReset_Device;
    QAction *actionBlank_Check;
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QProgressBar *progressBar;
    QFrame *line;
    QPlainTextEdit *plainTextEdit;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QMenu *menuProgrammer;
    QMenu *menu_Help;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QStringLiteral("MainWindowClass"));
        MainWindowClass->resize(439, 403);
        QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(MainWindowClass->sizePolicy().hasHeightForWidth());
        MainWindowClass->setSizePolicy(sizePolicy);
        MainWindowClass->setWindowTitle(QStringLiteral("USB HID Bootloader"));
        QIcon icon;
        icon.addFile(QStringLiteral(":/MainWindow/img/Microchip_logo.Ico"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindowClass->setWindowIcon(icon);
        actionOpen = new QAction(MainWindowClass);
        actionOpen->setObjectName(QStringLiteral("actionOpen"));
        actionOpen->setEnabled(false);
        QIcon icon1;
        icon1.addFile(QStringLiteral(":/MainWindow/img/open.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionOpen->setIcon(icon1);
        actionExit = new QAction(MainWindowClass);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        actionWrite_Device = new QAction(MainWindowClass);
        actionWrite_Device->setObjectName(QStringLiteral("actionWrite_Device"));
        actionWrite_Device->setEnabled(false);
        QIcon icon2;
        icon2.addFile(QStringLiteral(":/MainWindow/img/writetqfp.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionWrite_Device->setIcon(icon2);
        actionErase_Device = new QAction(MainWindowClass);
        actionErase_Device->setObjectName(QStringLiteral("actionErase_Device"));
        actionErase_Device->setEnabled(false);
        QIcon icon3;
        icon3.addFile(QStringLiteral(":/MainWindow/img/erasetqfp.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionErase_Device->setIcon(icon3);
        actionErase_Device->setVisible(false);
        action_About = new QAction(MainWindowClass);
        action_About->setObjectName(QStringLiteral("action_About"));
        QIcon icon4;
        icon4.addFile(QStringLiteral(":/MainWindow/img/help.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_About->setIcon(icon4);
        action_Verify_Device = new QAction(MainWindowClass);
        action_Verify_Device->setObjectName(QStringLiteral("action_Verify_Device"));
        action_Verify_Device->setEnabled(false);
        QIcon icon5;
        icon5.addFile(QStringLiteral(":/MainWindow/img/verify.png"), QSize(), QIcon::Normal, QIcon::Off);
        action_Verify_Device->setIcon(icon5);
        action_Verify_Device->setVisible(false);
        action_Settings = new QAction(MainWindowClass);
        action_Settings->setObjectName(QStringLiteral("action_Settings"));
        action_Settings->setEnabled(false);
        actionReset_Device = new QAction(MainWindowClass);
        actionReset_Device->setObjectName(QStringLiteral("actionReset_Device"));
        actionReset_Device->setEnabled(false);
        QIcon icon6;
        icon6.addFile(QStringLiteral(":/MainWindow/img/Reset.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionReset_Device->setIcon(icon6);
        actionBlank_Check = new QAction(MainWindowClass);
        actionBlank_Check->setObjectName(QStringLiteral("actionBlank_Check"));
        actionBlank_Check->setEnabled(false);
        QIcon icon7;
        icon7.addFile(QStringLiteral(":/MainWindow/img/BlankCheck.png"), QSize(), QIcon::Normal, QIcon::Off);
        actionBlank_Check->setIcon(icon7);
        actionBlank_Check->setVisible(false);
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        progressBar = new QProgressBar(centralWidget);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setValue(0);
        progressBar->setTextVisible(false);

        gridLayout->addWidget(progressBar, 0, 0, 1, 1);

        line = new QFrame(centralWidget);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        gridLayout->addWidget(line, 1, 0, 1, 1);

        plainTextEdit = new QPlainTextEdit(centralWidget);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setMinimumSize(QSize(350, 0));
        plainTextEdit->setReadOnly(true);

        gridLayout->addWidget(plainTextEdit, 2, 0, 1, 1);

        MainWindowClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindowClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 439, 19));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        menuProgrammer = new QMenu(menuBar);
        menuProgrammer->setObjectName(QStringLiteral("menuProgrammer"));
        menu_Help = new QMenu(menuBar);
        menu_Help->setObjectName(QStringLiteral("menu_Help"));
        MainWindowClass->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindowClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindowClass->setStatusBar(statusBar);
        mainToolBar = new QToolBar(MainWindowClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        mainToolBar->setMovable(false);
        mainToolBar->setFloatable(false);
        MainWindowClass->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuFile->menuAction());
        menuBar->addAction(menuProgrammer->menuAction());
        menuBar->addAction(menu_Help->menuAction());
        menuFile->addAction(actionOpen);
        menuFile->addSeparator();
        menuFile->addAction(actionExit);
        menuProgrammer->addSeparator();
        menuProgrammer->addAction(actionWrite_Device);
        menuProgrammer->addAction(action_Verify_Device);
        menuProgrammer->addAction(actionErase_Device);
        menuProgrammer->addAction(actionBlank_Check);
        menuProgrammer->addAction(actionReset_Device);
        menuProgrammer->addSeparator();
        menuProgrammer->addAction(action_Settings);
        menu_Help->addAction(action_About);
        mainToolBar->addAction(actionOpen);
        mainToolBar->addSeparator();
        mainToolBar->addAction(actionWrite_Device);
        mainToolBar->addAction(action_Verify_Device);
        mainToolBar->addAction(actionErase_Device);
        mainToolBar->addAction(actionBlank_Check);
        mainToolBar->addAction(actionReset_Device);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        actionOpen->setText(QApplication::translate("MainWindowClass", "&Import Firmware Image", 0));
#ifndef QT_NO_TOOLTIP
        actionOpen->setToolTip(QApplication::translate("MainWindowClass", "Import Firmware Image", 0));
#endif // QT_NO_TOOLTIP
        actionOpen->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+O", 0));
        actionExit->setText(QApplication::translate("MainWindowClass", "E&xit", 0));
        actionExit->setShortcut(QApplication::translate("MainWindowClass", "Ctrl+Q", 0));
        actionWrite_Device->setText(QApplication::translate("MainWindowClass", "Erase/Program/Verify Device", 0));
        actionErase_Device->setText(QApplication::translate("MainWindowClass", "Erase Device", 0));
        action_About->setText(QApplication::translate("MainWindowClass", "&About", 0));
        action_Verify_Device->setText(QApplication::translate("MainWindowClass", "&Verify Device", 0));
        action_Settings->setText(QApplication::translate("MainWindowClass", "&Settings...", 0));
        actionReset_Device->setText(QApplication::translate("MainWindowClass", "Reset Device", 0));
        actionBlank_Check->setText(QApplication::translate("MainWindowClass", "Blank Check", 0));
        menuFile->setTitle(QApplication::translate("MainWindowClass", "&File", 0));
        menuProgrammer->setTitle(QApplication::translate("MainWindowClass", "&Program", 0));
        menu_Help->setTitle(QApplication::translate("MainWindowClass", "&Help", 0));
        mainToolBar->setWindowTitle(QApplication::translate("MainWindowClass", "Display Toolbar", 0));
        Q_UNUSED(MainWindowClass);
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
