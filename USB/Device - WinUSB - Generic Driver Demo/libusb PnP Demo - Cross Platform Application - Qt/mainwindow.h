#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "usbcomm.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    Usbcomm* usbcomm;

public slots:
    void Update_GUI_Connection(bool isConnected);
    void Update_GUI_PushbuttonLabel(bool isPressed);

signals:
    void Toggle_LEDs_Button_Pressed();

private slots:
    void on_pushButton_clicked();
};

#endif // MAINWINDOW_H
