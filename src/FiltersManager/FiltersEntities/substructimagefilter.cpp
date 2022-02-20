#include "substructimagefilter.h"

SubstructImageFilter::SubstructImageFilter(QObject *parent)
    : ICheckFilter{parent}
{
    name = "Вычитание изображения";
    function = [&]()
    {
        imageCorrector->substractObjectImage();
    };
}
