#include "hardclipnoisefilter.h"

HardClipNoiseFilter::HardClipNoiseFilter(QObject *parent)
    : IComboFilter{parent}
{
    name = "Продвинутое удаление шума";
    QStringList listTypes;
    listTypes.append("Лёгкий");
    listTypes.append("Средний");
    listTypes.append("Сильный");

    setComboVariantsList(listTypes);  // сохраняем значения

    function = [&]()
    {
        imageCorrector->hardClipNoise(value, deleteType, ImageCorrector::NoiseDeleteColors::BLACK); // продвинутое удаление шумов
        imageCorrector->hardClipNoise(value, deleteType, ImageCorrector::NoiseDeleteColors::WHITE); // продвинутое удаление шумов
    };
}

void HardClipNoiseFilter::setComboBox(QComboBox *newComboBox)
{
    IComboFilter::setComboBox(newComboBox);
    connect(newComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &HardClipNoiseFilter::onComboBoxTypeIndexChanged);
}

void HardClipNoiseFilter::onComboBoxTypeIndexChanged(int index)
{
    deleteType = ImageCorrector::NoiseDeleteTypes(index);   // при изменении выбранного индекса, обновляем тип
}
