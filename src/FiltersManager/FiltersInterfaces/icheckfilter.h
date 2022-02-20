#ifndef ICHECKFILTER_H
#define ICHECKFILTER_H

#include "src/FiltersManager/ifilter.h"

class ICheckFilter : public IFilter
{
public:
    explicit ICheckFilter(QObject *parent = nullptr);
};

#endif // ICHECKFILTER_H
