#ifndef ISLIDERFILTER_H
#define ISLIDERFILTER_H

#include "src/FiltersManager/ifilter.h"
#include <QSlider>

class ISliderFilter : public IFilter
{
public:
    explicit ISliderFilter(QObject *parent = nullptr);

    int getValue() const;
    void setValue(int newValue);
    int getMinValue() const;
    void setMinValue(int newMinValue);
    int getMaxValue() const;
    void setMaxValue(int newMaxValue);
    QSlider *getSlider() const;
    void setSlider(QSlider *newSliderValue);

private slots:
    void onSliderValueChanged(int value);    // слот обработки изменения значения slider

protected:
    int value = 0;  // текущее значение
    int minValue = 0;   // минимальное значение
    int maxValue = 255;   // максимальное значение

    QSlider *slider = nullptr; // слайдер, с которого будут сниматься значения
};

#endif // ISLIDERFILTER_H
