#ifndef VISION_DETECT_FLOW_H
#define VISION_DETECT_FLOW_H

#include <QObject>
#include <QDebug>

class VisionDetectFlow : public QObject
{
    Q_OBJECT
public:
    VisionDetectFlow();
    ~VisionDetectFlow();

    void detect();
};

#endif