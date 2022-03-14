#ifndef IMAGECORRECTOR_H
#define IMAGECORRECTOR_H

#include <QObject>
#include <QImage>
#include <QDebug>
#include "imagedata.h"
#include "thread"

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
    void clipNoise(int clippingNoiseValue = -1);     // простое удаление шума
    void enchanceBlackColor(int blackEnchancement = -1);     // усилить чёрный цвет
    void invertPixels();    // инвертировать цвет пикселей выходного изобраения
    void hardClipNoise(int border, NoiseDeleteTypes type, NoiseDeleteColors colorType);   // усиленное удаление шума
//    void medianFilter();    // медианный фильтр по соседним пикселям
    void aliasing(int radius, int border, int blackBorderPercent = 50, int whiteBorderPercent = 50);  // выравнивание цветов по квадратам
    void medianRadiusFilter(int radius);  // медианный фильтр, основанный на радиусах

    void setImageOriginal(const QImage &value); // задать изображение фона
    void setImageObject(const QImage &value);   // задать изображение объекта
    QImage getResultImage() const;  // получить результирующее изображение

    void setThreadsCount(int newThreadsCount);  // установить кол-во потоков

    void setClippingNoiseValue(int newClippingNoiseValue);

    void setBlackEnchancement(int newBlackEnchancement);

private:
    void setPixelColor(QImage &image, int i, int j, QColor color);     // установить цвет пикселя для выходного изображения
    int getPixelBlackValue(QImage &image, int i, int j); // получить составляющую черного цвета пискселя
    QColor blackColor(int grayValue);   // возвращает цвет, соответствующий уровню серого
    QVector<QVector<bool>> getCorrectDataAroundPixels(QImage &image, int i, int j, int border, bool (*compare)(int comapredValue, int borderValue));    // возвращает матрицу 3*3, в которой содержится информация о соответствии пикселей вокруг данного, заданой границе
    bool** getCorrectDataAroundPixels(int **grayScaleMatrix, int i, int j, int border, bool (*compare)(int, int));
    QVector<QVector<int>> getMatrixAroundPixel(QImage &image, int i, int j);   // получить матрицу пикселей вокруг данного пикселя
    bool** getMatrixAroundPixel(int **grayScaleMatrix, int i, int j);   // получить матрицу пикселей вокруг данного пикселя

signals:

private:    // изображения
    ImageData imageOriginal; // изображение без фона
    ImageData imageObject; // изображение с объектом
    ImageData resultImage; // результирующее изображение

private:    // работа с потоками
//    QThread *threads = nullptr; // потоки для обработки данных в многопотоном режиме
    int threadsCount = 1;   // кол-во задействованных потоков
    int clippingNoiseValue = -1;    // значение удаления шумов
    int blackEnchancement = -1; // значение усиления чёрного

    void distributeToThreads(int startI, int endI, int startJ, int endJ, std::function<void(int i, int j)> function, int stepI = 0, int stepJ = 0);
    template<typename T>
    void memoryDoubleArrayFree(T **&array, int count1);    // очистка памяти двумерного массива
};

#endif // IMAGECORRECTOR_H
