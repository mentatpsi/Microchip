#include <qapplication.h>
#include "form1.h" 
#include <stdio.h> 



int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    Form1 w;
    w.show();
    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );
    return a.exec();
}
