#include "hid_pnp.h"

HID_PnP::HID_PnP(QObject *parent) : QObject(parent) {
    isConnected = false;
    pushbuttonStatus = false;
    potentiometerValue = 0;
    toggleLeds = 0;

    device = NULL;
    buf[0] = 0x00;
    buf[1] = 0x37;
    memset((void*)&buf[2], 0x00, sizeof(buf) - 2);
    
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(PollUSB()));

    timer->start(250);
}

HID_PnP::~HID_PnP() {
    disconnect(timer, SIGNAL(timeout()), this, SLOT(PollUSB()));
}

void HID_PnP::PollUSB()
{
    buf[0] = 0x00;
    memset((void*)&buf[2], 0x00, sizeof(buf) - 2);

    if (isConnected == false) {
        device = hid_open(0x04d8, 0x003f, NULL);

        if (device) {
            isConnected = true;
            hid_set_nonblocking(device, true);
            timer->start(15);
        }
    }
    else {
        if(toggleLeds == true) {
            toggleLeds = false;

            buf[1] = 0x80;

            if (hid_write(device, buf, sizeof(buf)) == -1)
            {
                CloseDevice();
                return;
            }

            buf[0] = 0x00;
            buf[1] = 0x37;
            memset((void*)&buf[2], 0x00, sizeof(buf) - 2);
        }

        if (hid_write(device, buf, sizeof(buf)) == -1)
        {
            CloseDevice();
            return;
        }
        if(hid_read(device, buf, sizeof(buf)) == -1)
        {
            CloseDevice();
            return;
        }
        if(buf[0] == 0x37) {
            potentiometerValue = (buf[2]<<8) + buf[1];
            buf[1] = 0x81;
        }
        else if(buf[0] == 0x81) {
            pushbuttonStatus = (buf[1] == 0x00);
            buf[1] = 0x37;
        }
    }

    hid_comm_update(isConnected, pushbuttonStatus, potentiometerValue);
}

void HID_PnP::toggle_leds() {
    toggleLeds = true;
}

void HID_PnP::CloseDevice() {
    hid_close(device);
    device = NULL;
    isConnected = false;
    pushbuttonStatus = false;
    potentiometerValue = 0;
    toggleLeds = 0;
    hid_comm_update(isConnected, pushbuttonStatus, potentiometerValue);
    timer->start(250);
}
