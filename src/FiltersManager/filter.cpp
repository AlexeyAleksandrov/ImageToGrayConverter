#include "filter.h"

Filter::Filter(QObject *parent)
    : QObject{parent}
{

}

const std::function<void()> &Filter::getFunction() const
{
    return function;
}

void Filter::setFunction(const std::function<void()> &newFunction)
{
    function = newFunction;
}

const QString &Filter::getName() const
{
    return name;
}

void Filter::setName(const QString &newName)
{
    name = newName;
}
