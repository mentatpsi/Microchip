#include <QtGui/QApplication>
#include "demoapp.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DemoApp w;
    w.show();

    return a.exec();
}
