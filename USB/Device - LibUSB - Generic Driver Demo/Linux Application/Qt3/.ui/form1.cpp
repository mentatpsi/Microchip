/****************************************************************************
** Form implementation generated from reading ui file 'form1.ui'
**
** Created: Thu Jun 2 20:06:09 2011
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#include "form1.h"

#include <qvariant.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include "../form1.ui.h"

/*
 *  Constructs a Form1 as a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'.
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
Form1::Form1( QWidget* parent, const char* name, bool modal, WFlags fl )
    : QDialog( parent, name, modal, fl )
{
    if ( !name )
	setName( "Form1" );
    setEnabled( TRUE );

    StateLabel = new QLabel( this, "StateLabel" );
    StateLabel->setEnabled( FALSE );
    StateLabel->setGeometry( QRect( 330, 80, 130, 26 ) );
    StateLabel->setTextFormat( QLabel::AutoText );

    Connect = new QPushButton( this, "Connect" );
    Connect->setGeometry( QRect( 20, 10, 101, 41 ) );

    PushButtonStatus = new QPushButton( this, "PushButtonStatus" );
    PushButtonStatus->setEnabled( FALSE );
    PushButtonStatus->setGeometry( QRect( 140, 70, 160, 41 ) );

    ToggleLed = new QPushButton( this, "ToggleLed" );
    ToggleLed->setEnabled( FALSE );
    ToggleLed->setGeometry( QRect( 140, 10, 160, 40 ) );
    languageChange();
    resize( QSize(448, 161).expandedTo(minimumSizeHint()) );
    clearWState( WState_Polished );

    // signals and slots connections
    connect( Connect, SIGNAL( clicked() ), this, SLOT( Connect_Device() ) );
    connect( ToggleLed, SIGNAL( clicked() ), this, SLOT( Toggle_LED() ) );
    connect( PushButtonStatus, SIGNAL( clicked() ), this, SLOT( Get_Push_Button_Status() ) );
}

/*
 *  Destroys the object and frees any allocated resources
 */
Form1::~Form1()
{
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void Form1::languageChange()
{
    setCaption( tr( "Form1" ) );
    StateLabel->setText( tr( "Unknown" ) );
    Connect->setText( tr( "Connect" ) );
    PushButtonStatus->setText( tr( "Push Button Status" ) );
    ToggleLed->setText( tr( "Toggle LED" ) );
}

