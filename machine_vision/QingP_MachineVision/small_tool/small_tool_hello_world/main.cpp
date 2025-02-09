#include <QtCore/QCoreApplication>
#include "HelloWorld/WG_HelloWorld.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    WG_HelloWorld clsUI;
    clsUI.show();

    return app.exec();
}
