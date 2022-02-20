#ifndef CLIPNOISEFILTER_H
#define CLIPNOISEFILTER_H

#include <src/FiltersManager/FiltersInterfaces/isliderfilter.h>

class ClipNoiseFilter : public ISliderFilter
{
public:
    explicit ClipNoiseFilter(QObject *parent = nullptr);
};

#endif // CLIPNOISEFILTER_H
