#ifndef SAVELOADIMAGE_SAVEONEIMAGE_H
#define SAVELOADIMAGE_SAVEONEIMAGE_H

#include "../../save_load_image_global.h"

#include <QObject>

class SAVE_LOAD_IMAGE_EXPORT SaveOneImage : public QObject
{
    Q_OBJECT
public:
    SaveOneImage();
    ~SaveOneImage();
};

#endif
