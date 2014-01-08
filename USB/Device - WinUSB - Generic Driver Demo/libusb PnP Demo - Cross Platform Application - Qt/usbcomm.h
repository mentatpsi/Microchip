#ifndef LIBUSB_DEMO_H
#define LIBUSB_DEMO_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QFuture>


#include <wchar.h>
#include <string.h>
#include <stdlib.h>

#include "libusb.h"

#define MAX_STR 65

class Usbcomm : public QObject
//class Usbcomm : public QThread
{
    Q_OBJECT
public:
    explicit Usbcomm(QObject *parent = 0);
    ~Usbcomm();
    //bool GetConnectionState(void);
    //bool GetPushbuttonState(void);

signals:
    void comm_update_connection(bool isConnected);
    void comm_update_pushbutton(bool isPushbuttonPressed);

public slots:
    void SendToggleLEDs(void);
    void PollPushbuttonState(void);
    void PollUSBConnection(volatile bool*); //Note: should be run with QtConcurrent as a separate thread

private:
    bool isConnected;
    //bool pushbuttonStatus;
    bool toggleLeds;
    bool isPushbuttonPressed;

    //unsigned char buf[MAX_STR];

    //void CloseDevice(void);
};

#endif // LIBUSB_DEMO_H
