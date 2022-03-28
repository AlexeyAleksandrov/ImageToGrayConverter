#ifndef IMAGECORRECTRFILTERPARAMS_H
#define IMAGECORRECTRFILTERPARAMS_H

#include <src/ImageHandlers/imagecorrector.h>
#include <src/ImageHandlers/imagedata.h>

// класс хранит параметры фильтров для обработки участка изображения

class ImageCorrectrFilterParams
{
public:
    ImageCorrectrFilterParams();

public:
    // параметры включения и выключения фильтров
    bool needSubstructImage = false;
    bool needHardDeleteNoise = false;
    bool needHardDeleteWhiteColor = false;
    bool needHardDeleteBlackColor = false;
    bool needMedianFilter = false;
    bool needAverageFilter = false;
    bool needAliasing = false;
    bool needColorInversion = false;
    bool needAliasingVisualisation = false;

    // параметры диапазона выборки значений
    int widthStart = 0;     // Х начала рассчёта в матрице изображения
    int heightStart = 0;    // Y начала рассчёта в матрице изображения

    int widthEnd = 0;   // X конца рассчёта в матрице изображения
    int heightEnd = 0;  // Y конца рассчёта в матрице изображения

    // системные параметры
    int threadsCount = 1;   // кол-во задействованных потоков
    int repeatOffset = 0;   // кол-во повторов наложения фильтров со сдвигом радиуса и наложения методом Исключающее ИЛИ

    // параметры фильтров
    int clippingNoiseValue = -1;    // значение удаления шумов
    int blackEnchancement = -1; // значение усиления чёрного

    int hardDeleteNoiseBorder = 1;  // граница для усиленного удаления шума
    ImageCorrector::NoiseDeleteTypes hardDeleteNoiseDeleteType; // тип усиленного удаления шума

    int medianFilter_radius = 1;    // радиус медианного фильтра
    int averageFilter_radius = 1;   // радиус среднеарифметического фильтра

    int aliasingRadius = 1;     // радиус для пикселизации
    int aliasingBorder = 126;   // 255/2    - ставим половину, хотя тим изображение и так 0 или 255. Можно оббое отличное от грничных знаечние
    int aliasingBlackBorder = -1;   // граница чёрного цвета для усиленного удаления шумов
    int aliasingWhiteBorder = -1;   // граница белого цвета для усиленного удаления шумов
};

#endif // IMAGECORRECTRFILTERPARAMS_H
