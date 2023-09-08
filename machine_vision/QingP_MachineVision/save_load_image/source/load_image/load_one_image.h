#ifndef SAVELOADIMAGE_LOADONEIMAGE_H
#define SAVELOADIMAGE_LOADONEIMAGE_H

#include "../../save_load_image_global.h"

#include <QObject>

class SAVE_LOAD_IMAGE_EXPORT LoadOneImage : public QObject
{
    Q_OBJECT
public:
    LoadOneImage();
    ~LoadOneImage();
};

#endif
