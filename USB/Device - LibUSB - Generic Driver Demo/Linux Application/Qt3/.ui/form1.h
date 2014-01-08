/****************************************************************************
** Form interface generated from reading ui file 'form1.ui'
**
** Created: Thu Jun 2 20:06:08 2011
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef FORM1_H
#define FORM1_H

#include <qvariant.h>
#include <qdialog.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QLabel;
class QPushButton;

class Form1 : public QDialog
{
    Q_OBJECT

public:
    Form1( QWidget* parent = 0, const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~Form1();

    QLabel* StateLabel;
    QPushButton* Connect;
    QPushButton* PushButtonStatus;
    QPushButton* ToggleLed;

public slots:
    virtual void Connect_Device( void );
    virtual void Toggle_LED();
    virtual void Get_Push_Button_Status();
    virtual void close();

protected:

protected slots:
    virtual void languageChange();

private:
    unsigned char Connection_Status;
    char tmp[64];

};

#endif // FORM1_H
