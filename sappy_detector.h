#include <QString>
#ifndef SAPPY_DETECTOR_H
#define SAPPY_DETECTOR_H


class sappy_detector
{
public:
    static int findEngine(QString romPath, int adr);
};

#endif // SAPPY_DETECTOR_H
