#ifndef IMAGECORRECTOR_H
#define IMAGECORRECTOR_H

#include <QObject>
#include <QImage>
#include <QDebug>
#include "imagedata.h"

#define BLACK_GRAY_LEVEL 0
#define WHITE_GRAY_LEVEL 255

class ImageCorrector : public QObject
{
    Q_OBJECT
public:
    explicit ImageCorrector(QObject *parent = nullptr);

public:
    enum NoiseDeleteTypes
    {
        LOW,
        MEDIUM,
        HIGH
    };

    enum NoiseDeleteColors
    {
        WHITE,
        BLACK
    };

public:

    void substractObjectImage();     // вычесть изображение объекта
    void clipNoise(int clippingNoiseValue);     // простое удаление шума
    void enchanceBlackColor(int blackEnchancement);     // усилить чёрный цвет
    void invertPixels();    // инвертировать цвет пикселей выходного изобраения
    void hardClipNoise(int border, NoiseDeleteTypes type, NoiseDeleteColors colorType);   // усиленное удаление шума
    void medianFilter();    // медианный фильтр по соседним пикселям

    void setImageOriginal(const QImage &value); // задать изображение фона
    void setImageObject(const QImage &value);   // задать изображение объекта
    QImage getResultImage() const;  // получить результирующее изображение

private:
    void setPixelColor(QImage &image, int i, int j, QColor color);     // установить цвет пикселя для выходного изображения
    int getPixelBlackValue(QImage &image, int i, int j); // получить составляющую черного цвета пискселя
    QColor blackColor(int grayValue);   // возвращает цвет, соответствующий уровню серого
    QVector<QVector<bool>> getCorrectDataAroundPixels(QImage &image, int i, int j, int border, bool (*compare)(int comapredValue, int borderValue));    // возвращает матрицу 3*3, в которой содержится информация о соответствии пикселей вокруг данного, заданой границе
    bool** getCorrectDataAroundPixels(int **grayScaleMatrix, int i, int j, int border, bool (*compare)(int, int));
    QVector<QVector<int>> getMatrixAroundPixel(QImage &image, int i, int j);   // получить матрицу пикселей вокруг данного пикселя
    bool** getMatrixAroundPixel(int **grayScaleMatrix, int i, int j);   // получить матрицу пикселей вокруг данного пикселя

signals:

private:
//    QImage imageOriginal;   // изображение без фона
//    QImage imageObject; // изображение с объектом
//    QImage resultImage; // результирующее изображение

//    ImageData resultImageData;  // информация об результирующем изображении
    ImageData imageOriginal; // изображение без фона
    ImageData imageObject; // изображение с объектом
    ImageData resultImage; // результирующее изображение

};

#endif // IMAGECORRECTOR_H
