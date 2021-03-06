#ifndef IMAGECORRECTOR_H
#define IMAGECORRECTOR_H

#include <QObject>
#include <QImage>
#include <QDebug>
#include "imagedata.h"
#include "thread"
#include "src/ImageFilters/imagecorrectrfilterparams.h"
#include "imagecorrectorenums.h"

#define BLACK_GRAY_LEVEL 0
#define WHITE_GRAY_LEVEL 255

class ImageCorrector : public QObject
{
    Q_OBJECT
public:
    explicit ImageCorrector(QObject *parent = nullptr);
//    void setFilter(ImageCorrectrFilterParams filter);   // задать фильтр, по которому будет выполнена обработка изображения


public:

    void substractObjectImage();     // вычесть изображение объекта
    void clipNoise(int clippingNoiseValue = -1);     // простое удаление шума
    void enchanceBlackColor(int blackEnchancement = -1);     // усилить чёрный цвет
    void invertPixels();    // инвертировать цвет пикселей выходного изобраения
    void hardClipNoise(int border, ImageCorrectorEnums::NoiseDeleteTypes type, ImageCorrectorEnums::NoiseDeleteColors colorType);   // усиленное удаление шума
//    void medianFilter();    // медианный фильтр по соседним пикселям
    void aliasing(int radius, int border, int blackBorderPercent = 50, int whiteBorderPercent = 50);  // выравнивание цветов по квадратам
    void medianRadiusFilter(int radius);  // медианный фильтр, основанный на радиусах
    void averageFilter(int radius); // среднеарифметический фильтр

    void setImageOriginal(const QImage &value); // задать изображение фона
    void setImageObject(const QImage value);   // задать изображение объекта
    QImage getResultImage() const;  // получить результирующее изображение

//    void setThreadsCount(int newThreadsCount);  // установить кол-во потоков

//    void setClippingNoiseValue(int newClippingNoiseValue);

//    void setBlackEnchancement(int newBlackEnchancement);

    void setFilter(const ImageCorrectrFilterParams &newFilter);

    void setImage_min_x(int newImage_min_x);
    void setImage_min_y(int newImage_min_y);
    void setImage_max_x(int newImage_max_x);
    void setImage_max_y(int newImage_max_y);

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
    ImageCorrectrFilterParams filter;   // фильтр, по которому будет выполнена обработка изображения

    int image_min_x = 0;
    int image_min_y = 0;
    int image_max_x = 0;
    int image_max_y = 0;

private:    // работа с потоками
//    QThread *threads = nullptr; // потоки для обработки данных в многопотоном режиме
//    int threadsCount = 1;   // кол-во задействованных потоков
//    int clippingNoiseValue = -1;    // значение удаления шумов
//    int blackEnchancement = -1; // значение усиления чёрного

    void distributeToThreads(int startI, int endI, int startJ, int endJ, std::function<void(int i, int j)> function, int stepI = 1, int stepJ = 1);
    template<typename T>
    void memoryDoubleArrayFree(T **&array, int count1);    // очистка памяти двумерного массива
};

#endif // IMAGECORRECTOR_H
