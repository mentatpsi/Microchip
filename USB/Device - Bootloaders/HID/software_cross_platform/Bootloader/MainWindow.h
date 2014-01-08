/************************************************************************
* Copyright (c) 2009-2010,  Microchip Technology Inc.
*
* Microchip licenses this software to you solely for use with Microchip
* products.  The software is owned by Microchip and its licensors, and
* is protected under applicable copyright laws.  All rights reserved.
*
* SOFTWARE IS PROVIDED "AS IS."  MICROCHIP EXPRESSLY DISCLAIMS ANY
* WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT
* NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
* FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.  IN NO EVENT SHALL
* MICROCHIP BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR
* CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR
* EQUIPMENT, COST OF PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY
* OR SERVICES, ANY CLAIMS BY THIRD PARTIES (INCLUDING BUT NOT LIMITED
* TO ANY DEFENSE THEREOF), ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION,
* OR OTHER SIMILAR COSTS.
*
* To the fullest extent allowed by law, Microchip and its licensors
* liability shall not exceed the amount of fees, if any, that you
* have paid directly to Microchip to use this software.
*
* MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE
* OF THESE TERMS.
*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QFileInfo>
#include <QFileSystemWatcher>
#include <QtCore/QProcess>
#include <QtWidgets/QMenu>
#include <QFuture>

#include "Comm.h"
#include "DeviceData.h"
#include "Device.h"
#include "ImportExportHex.h"

namespace Ui
{
    class MainWindowClass;
}

#define MAX_RECENT_FILES 3

/*!
 * The main Serial Bootloader GUI window.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void GetQuery(void);
    void LoadFile(QString fileName);

    void EraseDevice(void);
    void BlankCheckDevice(void);
    void WriteDevice(void);
    void VerifyDevice(void);

    void setBootloadBusy(bool busy);

signals:
    void IoWithDeviceCompleted(QString msg, Comm::ErrorCode, double time);
    void IoWithDeviceStarted(QString msg);
    void AppendString(QString msg);
    void SetProgressBar(int newValue);

public slots:
    void Connection(void);
    void openRecentFile(void);
    void IoWithDeviceComplete(QString msg, Comm::ErrorCode, double time);
    void IoWithDeviceStart(QString msg);
    void AppendStringToTextbox(QString msg);
    void UpdateProgressBar(int newValue);

protected:
    Comm* comm;
    DeviceData* deviceData;
    DeviceData* hexData;
    Device* device;

    QFuture<void> future;

    QString fileName, watchFileName;
    QFileSystemWatcher* fileWatcher;
    QTimer *timer;

    bool writeFlash;
    bool writeEeprom;
    bool writeConfig;
    bool eraseDuringWrite;
    bool hexOpen;

    void setBootloadEnabled(bool enable);

    void UpdateRecentFileList(void);

    Comm::ErrorCode RemapInterruptVectors(Device* device, DeviceData* deviceData);

private:
    Ui::MainWindowClass *ui;
    QLabel deviceLabel;

    int failed;
    QAction *recentFiles[MAX_RECENT_FILES];

    bool wasBootloaderMode;

private slots:
    void on_actionBlank_Check_triggered();
    void on_actionReset_Device_triggered();
    void on_action_Settings_triggered();
    void on_action_Verify_Device_triggered();
    void on_action_About_triggered();
    void on_actionWrite_Device_triggered();
    void on_actionOpen_triggered();
    void on_actionErase_Device_triggered();
    void on_actionExit_triggered();
};

#endif // MAINWINDOW_H
