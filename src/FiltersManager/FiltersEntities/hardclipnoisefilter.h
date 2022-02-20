#ifndef HARDCLIPNOISEFILTER_H
#define HARDCLIPNOISEFILTER_H

#include <src/FiltersManager/FiltersInterfaces/icombofilter.h>
#include <QComboBox>

class HardClipNoiseFilter : public IComboFilter
{
public:
    explicit HardClipNoiseFilter(QObject *parent = nullptr);
    void setComboBox(QComboBox *newComboBox) override;  // переопределяем функцию установки ComboBox

private:
    ImageCorrector::NoiseDeleteTypes deleteType;    // тип удаления шумов

private slots:
    void onComboBoxTypeIndexChanged(int index); // слот изменения выборанного индекса в comboBox, отвечающего за тип
};

#endif // HARDCLIPNOISEFILTER_H
