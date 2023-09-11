#include <QtCore/QCoreApplication>

#include "source/vision_detect_flow/vision_detect_flow.h"
#include "source/vision_detect_flow/vision_detect_flow_thread.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // 非多线程版本
    //VisionDetectFlow clsVisionDetectFlow;
    //clsVisionDetectFlow.detect();

    // 多线程版本
    VisionDetectFlowThread clsVisionDetectFlowThread;
    clsVisionDetectFlowThread.start();

    return a.exec();
}
