#ifndef IFILTER_H
#define IFILTER_H

#include <QObject>
#include <QCheckBox>
#include "src/ImageHandlers/imagecorrector.h"
#include <QDebug>

class IFilter : public QObject
{
    Q_OBJECT
public:
    explicit IFilter(QObject *parent = nullptr);
    IFilter(const IFilter &object)
    {
        QObject(object.parent());
    }

    IFilter& operator=(const IFilter &other)
    {
        return *this;
    }

    enum types  // типы фильтров для созданяи на форме
    {
        CHECK_FILTER,   // checkBox фильтр - просто галочка
        SLIDER_FILTER,  // slider фильтр - значение + слайдер
        COMBO_FILTER    // comboBox фильтр - выпадающий список с вариантами + слайдер + значение
    };

signals:

private:    // значения для UI
    bool isUseFilter = false;   // флаг, должен-ли фильтр использоваться, или нет
    QCheckBox *checkBoxUse = nullptr;   // указатель на checkBox, который используется для включений и выключения фильтра

protected:
    QString name;   // название фильтра
    types type; // Тип фильтра, необходим для правильного преобразования
    std::function<void()> function; // функция, которую выполнит фильтр
    ImageCorrector *imageCorrector = nullptr;   // обработчик изображении для применения фильтра

public:
    const QString &getName() const;
    void setName(const QString &newName);
    const std::function<void()> &getFunction() const;
    void setFunction(const std::function<void()> &newFunction);
    bool getIsUseFilter() const;
    void setIsUseFilter(bool newIsUseFilter);
    QCheckBox *getCheckBoxUse() const;
    void setCheckBoxUse(QCheckBox *newCheckBoxUse);
    void setImageCorrector(ImageCorrector *newImageCorrector);
};

#endif // IFILTER_H
