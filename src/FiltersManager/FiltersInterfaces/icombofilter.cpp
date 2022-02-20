#include "icombofilter.h"

IComboFilter::IComboFilter(QObject *parent)
    : ISliderFilter{parent}
{
    type = COMBO_FILTER;
}

QComboBox *IComboFilter::getComboBox() const
{
    return comboBox;
}

void IComboFilter::setComboBox(QComboBox *newComboBox)
{
    comboBox = newComboBox;
    comboBox->addItems(comboVariantsList);
}

const QStringList &IComboFilter::getComboVariantsList() const
{
    return comboVariantsList;
}

void IComboFilter::setComboVariantsList(const QStringList &newComboVariantsList)
{
    comboVariantsList = newComboVariantsList;
    if(comboBox != nullptr)
    {
        comboBox->addItems(comboVariantsList);
    }
}
