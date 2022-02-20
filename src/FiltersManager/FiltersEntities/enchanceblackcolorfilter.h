#ifndef ENCHANCEBLACKCOLORFILTER_H
#define ENCHANCEBLACKCOLORFILTER_H

#include <src/FiltersManager/FiltersInterfaces/isliderfilter.h>

class EnchanceBlackColorFilter : public ISliderFilter
{
public:
    explicit EnchanceBlackColorFilter(QObject *parent = nullptr);
};

#endif // ENCHANCEBLACKCOLORFILTER_H
