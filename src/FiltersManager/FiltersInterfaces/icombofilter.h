#ifndef ICOMBOFILTER_H
#define ICOMBOFILTER_H

//#include "src/FiltersManager/ifilter.h"
#include "isliderfilter.h"
#include <QObject>
#include <QComboBox>

class IComboFilter : public ISliderFilter
{
public:
    explicit IComboFilter(QObject *parent = nullptr);

    QComboBox *getComboBox() const;
    virtual void setComboBox(QComboBox *newComboBox);
    const QStringList &getComboVariantsList() const;
    void setComboVariantsList(const QStringList &newComboVariantsList);

protected:
    QComboBox *comboBox = nullptr;
    QStringList comboVariantsList;
};

#endif // ICOMBOFILTER_H
