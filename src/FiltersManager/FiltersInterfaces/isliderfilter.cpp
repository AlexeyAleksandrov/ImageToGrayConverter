#include "isliderfilter.h"

ISliderFilter::ISliderFilter(QObject *parent)
    : IFilter{parent}
{
    type = SLIDER_FILTER;
}

int ISliderFilter::getValue() const
{
    return value;
}

void ISliderFilter::setValue(int newValue)
{
    if(newValue >= minValue && newValue <= maxValue)    // если новое значение подходит под диапазон
    {
        value = newValue;

        if(slider != nullptr)
        {
            slider->setValue(value);
        }
    }
    else
    {
        qDebug() << "Нкорректное значение " << value << minValue << maxValue;
    }
}

int ISliderFilter::getMinValue() const
{
    return minValue;
}

void ISliderFilter::setMinValue(int newMinValue)
{
    minValue = newMinValue;
    if(slider != nullptr)
    {
        slider->setMaximum(minValue);
    }
}

int ISliderFilter::getMaxValue() const
{
    return maxValue;
}

void ISliderFilter::setMaxValue(int newMaxValue)
{
    maxValue = newMaxValue;
    if(slider != nullptr)
    {
        slider->setMaximum(maxValue);
    }
}

QSlider *ISliderFilter::getSlider() const
{
    return slider;
}

void ISliderFilter::setSlider(QSlider *newSliderValue)
{
    slider = newSliderValue;
    slider->setMaximum(minValue);
    slider->setMaximum(maxValue);
    slider->setValue(value);
    connect(slider, &QSlider::valueChanged, this, &ISliderFilter::onSliderValueChanged);    // соединяем сигнал изменения значения со слотом
}

void ISliderFilter::onSliderValueChanged(int value)
{
    this->value = value;    // сохраняем значение
}
