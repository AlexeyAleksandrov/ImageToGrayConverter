#include "enchanceblackcolorfilter.h"

EnchanceBlackColorFilter::EnchanceBlackColorFilter(QObject *parent)
    : ISliderFilter{parent}
{
    name = "Усиление чёрного";
    function = [&]()
    {
        imageCorrector->enchanceBlackColor(value);
    };
}
