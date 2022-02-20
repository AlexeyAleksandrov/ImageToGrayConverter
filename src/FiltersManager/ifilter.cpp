#include "ifilter.h"

IFilter::IFilter(QObject *parent)
    : QObject{parent}
{

}

void IFilter::setImageCorrector(ImageCorrector *newImageCorrector)
{
    imageCorrector = newImageCorrector;
}

QCheckBox *IFilter::getCheckBoxUse() const
{
    return checkBoxUse;
}

void IFilter::setCheckBoxUse(QCheckBox *newCheckBoxUse)
{
    checkBoxUse = newCheckBoxUse;
}

const std::function<void()> &IFilter::getFunction() const
{
    return function;
}

void IFilter::setFunction(const std::function<void()> &newFunction)
{
    function = newFunction;
}

const QString &IFilter::getName() const
{
    return name;
}

void IFilter::setName(const QString &newName)
{
    name = newName;
}

bool IFilter::getIsUseFilter() const
{
    return isUseFilter;
}

void IFilter::setIsUseFilter(bool newIsUseIFilter)
{
    isUseFilter = newIsUseIFilter;
}
