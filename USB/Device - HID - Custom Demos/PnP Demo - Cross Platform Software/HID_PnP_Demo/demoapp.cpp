#include "demoapp.h"
#include "ui_demoapp.h"

DemoApp::DemoApp(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DemoApp)
{
    ui->setupUi(this);

    plugNPlay = new HID_PnP();

    connect(this, SIGNAL(toggle_leds_button_pressed()), plugNPlay, SLOT(toggle_leds()));
    connect(plugNPlay, SIGNAL(hid_comm_update(bool, bool, int)), this, SLOT(update_gui(bool, bool, int)));
}

DemoApp::~DemoApp()
{
    disconnect(this, SIGNAL(toggle_leds_button_pressed()), plugNPlay, SLOT(toggle_leds()));
    disconnect(plugNPlay, SIGNAL(hid_comm_update(bool, bool, int)), this, SLOT(update_gui(bool, bool, int)));
    delete ui;
    delete plugNPlay;
}

void DemoApp::on_pushButton_clicked()
{
    emit toggle_leds_button_pressed();
}

void DemoApp::update_gui(bool isConnected, bool isPressed, int potentiometerValue)
{
    if(isConnected)
    {
        ui->label_2->setEnabled(true);
        ui->pushButton->setEnabled(true);
        ui->pushbuttonStatus->setEnabled(true);
        ui->progressBar->setEnabled(true);

        ui->deviceConnectedStatus->setText("Device Found: AttachedState = TRUE");

        if(isPressed)
            ui->pushbuttonStatus->setText("Pushbutton State: Pressed");
        else
            ui->pushbuttonStatus->setText("Pushbutton State: Not Pressed");

        ui->progressBar->setValue(potentiometerValue);
    }
    else
    {
        ui->label_2->setEnabled(false);
        ui->pushButton->setEnabled(false);
        ui->pushbuttonStatus->setEnabled(false);
        ui->progressBar->setEnabled(false);

        ui->deviceConnectedStatus->setText("Device Not Detected: Verify Connection/Correct Firmware");
        ui->pushbuttonStatus->setText("Pushbutton State: Unknown");
        ui->progressBar->setValue(0);
    }
}
