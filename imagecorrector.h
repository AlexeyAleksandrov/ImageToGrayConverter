#ifndef IMAGECORRECTOR_H
#define IMAGECORRECTOR_H

#include <QObject>
#include <QImage>
#include <QDebug>

class ImageCorrector : public QObject
{
    Q_OBJECT
public:
    explicit ImageCorrector(QObject *parent = nullptr);

    void subtractObjectImage();     // вычесть изображение объекта
    void clipNoise(int clippingNoiseValue);     // простое удаление шума
    void enchanceBlackColor(int blackEnchancement);     // усилить чёрный цвет
    void invertPixels();    // инвертировать цвет пикселей выходного изобраения

    void setImageOriginal(const QImage &value); // задать изображение фона
    void setImageObject(const QImage &value);   // задать изображение объекта
    QImage getResultImage() const;  // получить результирующее изображение

private:
    void setPixelColor(QImage &image, int i, int j, QColor color);     // установить цвет пикселя для выходного изображения
    QColor blackColor(int grayValue);   // возвращает цвет, соответствующий уровню серого

signals:

private:
    QImage imageOriginal;   // изображение без фона
    QImage imageObject; // изображение с объектом
    QImage resultImage; // результирующее изображение
};

#endif // IMAGECORRECTOR_H
