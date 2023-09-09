#include <QtCore/QCoreApplication>

#include "source/vision_detect_flow/vision_detect_flow.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    VisionDetectFlow clsVisionDetectFlow;

    clsVisionDetectFlow.detect();

    return a.exec();
}
