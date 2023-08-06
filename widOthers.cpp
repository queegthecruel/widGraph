#include "widOthers.h"

painterAntiAl::painterAntiAl(QPaintDevice *device):
    QPainter(device)
{
    setRenderHint(QPainter::Antialiasing);
}
