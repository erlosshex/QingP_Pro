#include <QtCore/QCoreApplication>

#include "source/vision_detect_flow/vision_detect_flow.h"
#include "source/vision_detect_flow/vision_detect_flow_thread.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    // �Ƕ��̰߳汾
    //VisionDetectFlow clsVisionDetectFlow;
    //clsVisionDetectFlow.detect();

    // ���̰߳汾
    VisionDetectFlowThread clsVisionDetectFlowThread;
    clsVisionDetectFlowThread.start();

    return a.exec();
}
