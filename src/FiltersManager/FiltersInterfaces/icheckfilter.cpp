#include "icheckfilter.h"

ICheckFilter::ICheckFilter(QObject *parent)
    : IFilter{parent}
{
    type = CHECK_FILTER;
}
