#ifndef SUBSTRUCTIMAGEFILTER_H
#define SUBSTRUCTIMAGEFILTER_H

#include <src/FiltersManager/FiltersInterfaces/icheckfilter.h>

class SubstructImageFilter : public ICheckFilter
{
public:
    explicit SubstructImageFilter(QObject *parent = nullptr);
};

#endif // SUBSTRUCTIMAGEFILTER_H
