#include <QtCore/QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Hello, World!";
    qDebug() << "Hello, QingP Machine Vision!";

    return a.exec(); 
}
