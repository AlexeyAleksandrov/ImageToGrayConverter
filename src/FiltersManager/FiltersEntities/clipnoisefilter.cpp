#include "clipnoisefilter.h"

ClipNoiseFilter::ClipNoiseFilter(QObject *parent)
    : ISliderFilter{parent}
{
    name = "Удаление шума";
    function = [&]()
    {
        imageCorrector->clipNoise();
    };
}
