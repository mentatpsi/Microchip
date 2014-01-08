#ifndef DEMOAPP_H
#define DEMOAPP_H

#include <QMainWindow>
#include "hid_pnp.h"

namespace Ui {
    class DemoApp;
}

class DemoApp : public QMainWindow
{
    Q_OBJECT

public:
    explicit DemoApp(QWidget *parent = 0);
    ~DemoApp();

private:
    Ui::DemoApp *ui;
    HID_PnP *plugNPlay;

public slots:
    void update_gui(bool isConnected, bool isPressed, int potentiometerValue);

signals:
    void toggle_leds_button_pressed();

private slots:
    void on_pushButton_clicked();
};

#endif // DEMOAPP_H
