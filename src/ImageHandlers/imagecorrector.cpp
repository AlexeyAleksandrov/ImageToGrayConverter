#include "imagecorrector.h"

ImageCorrector::ImageCorrector(QObject *parent) : QObject(parent)
{

}

void ImageCorrector::substractObjectImage()
{
    auto imageOriginalMatrix = imageOriginal.getGrayScaleMatrix();  // матрица пикселей исходного изображения
    auto imageObjectMatrix = imageObject.getGrayScaleMatrix();  // матрица пикселей изображения объекта
    auto imageResultMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения

    for (int i=0; i<imageOriginal.getWidth(); i++) // проходим по ширине
    {
        for (int j=0; j<imageOriginal.getHeight(); j++)    // проходим по высоте
        {
            int blackOriginal = imageOriginalMatrix[i][j];  // уровень черного в оригинале
            int blackObject = imageObjectMatrix[i][j];  // уровень черного в объекте

            int blackSub = abs(blackOriginal - blackObject);    // вычитаем фон
            imageResultMatrix[i][j] = blackSub; // устанавливаем цвет пикселя
        }
    }
}

void ImageCorrector::clipNoise(int clippingNoiseValue)
{
    if(clippingNoiseValue == -1)
    {
        clippingNoiseValue = filter.clippingNoiseValue;
    }
    auto imageResultMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения

    std::function<void(int i, int j)> function = [&](int i, int j)
    {
        int black = imageResultMatrix[i][j];   // получаем уровень черного
        if(black < clippingNoiseValue)   // обрезаем шумы в соответсвии с указанным уровнем
        {
            black = 0;
        }

        imageResultMatrix[i][j] = black;    // устанавливаем цвет пикселя
    };

    distributeToThreads(image_min_x, image_max_x, image_min_y, image_max_y, function);
}

void ImageCorrector::enchanceBlackColor(int blackEnchancement)
{
    if(blackEnchancement == -1)
    {
        blackEnchancement = 255 - filter.blackEnchancement;
    }
    auto imageResultMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения

    std::function<void(int i, int j)> function = [&](int i, int j)
    {
        int black = imageResultMatrix[i][j];   // получаем уровень черного
        if(black > blackEnchancement)    // усиливаем черный цвет
        {
            black = 255;
        }

        imageResultMatrix[i][j] = black;    // устанавливаем цвет пикселя
    };

    distributeToThreads(image_min_x, image_max_x, image_min_y, image_max_y, function); // распределяем по потокам
}

void ImageCorrector::invertPixels()
{
    QImage img = resultImage.getImage();
    img.invertPixels();
    resultImage.setImage(img);
}

void ImageCorrector::hardClipNoise(int border, ImageCorrectorEnums::NoiseDeleteTypes type, ImageCorrectorEnums::NoiseDeleteColors colorType)
{
    auto imageResultMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения

    std::function<void(int i, int j)> function = [&](int i, int j)
    {
        int black = imageResultMatrix[i][j];    // получаем уровень черного

        if(black <= border) // если цвет попадает под границу для сглаживания шума
        {
            bool (*copmarator)(int comparedValue, int borderValue) = nullptr;   // функция сравнения значения с границей
            int replasePixelColorGrayLevel = 0; // цвет, на который будет заменён цвет пикселя в случае сглаживания
            if(colorType == ImageCorrectorEnums::WHITE)  // условие для белового цвета
            {
                copmarator = [](int comparedValue, int borderValue)
                {
                    return comparedValue <= borderValue;    // значение сранивается на <=
                };

                replasePixelColorGrayLevel = BLACK_GRAY_LEVEL;
            }
            else // условие сравнения для чёрного цвета
            {
                copmarator = [](int comparedValue, int borderValue)
                {
                    return comparedValue > borderValue; // значения сравниваются в большую сторону
                };

                replasePixelColorGrayLevel = WHITE_GRAY_LEVEL;
            }
            auto correctMatrix = getCorrectDataAroundPixels(imageResultMatrix, i, j, border, copmarator);   // получаем матрицу коорректности

            // лёгкое удаление (8 пикселей)
            if(type == ImageCorrectorEnums::LOW)
            {
                if(correctMatrix[0][0] && correctMatrix[0][1] && correctMatrix[0][2]
                         && correctMatrix[1][0]  && correctMatrix[1][2]
                         && correctMatrix[2][0] && correctMatrix[2][1] && correctMatrix[2][2])  // если под условия подходят все значения вокруг
                {
                    imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем цвет пикселю
                }
            }

            // среднее удаление (4 пикселя)
            if(type == ImageCorrectorEnums::MEDIUM)
            {
                // крестом
                if(correctMatrix[0][0] && correctMatrix[0][2]
                         && correctMatrix[2][0] && correctMatrix[2][2])  // если под условия подходят все значения вокруг, расположенные по диагонале
                {
                    imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем цвет пикселю
                }
                // плюсом
                if(correctMatrix[0][1]
                         && correctMatrix[1][0]  && correctMatrix[1][2]
                         && correctMatrix[2][1])  // если под условия подходят все значения вокруг, расположенные слева, справа, сверху и снизу
                {
                    imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем цвет пикселю
                }
            }

            // сильное удаление (2 пикселя)
            if(type == ImageCorrectorEnums::HIGH)
            {
                // горизонталь
                if(correctMatrix[1][0] && correctMatrix[1][2])  // если под условия подходят все значения, расположенные слева и справа
                {
                    imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем цвет пикселю
                }
                // вертикаль
                if(correctMatrix[0][1] && correctMatrix[2][1])  // если под условия подходят все значения, расположенные сверху и снизу
                {
                    imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем цвет пикселю
                }
                // диагональ слева направо
                if(correctMatrix[0][0] && correctMatrix[2][2])  // если под условия подходят все значения, расположенные слева и справа
                {
                    imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем цвет пикселю
                }
                // диагональ справа налево
                if(correctMatrix[0][2] && correctMatrix[2][0])  // если под условия подходят все значения, расположенные сверху и снизу
                {
                    imageResultMatrix[i][j] = replasePixelColorGrayLevel;   // устанавливаем цвет пикселю
                }
            }

            memoryDoubleArrayFree(correctMatrix, 3);    // очищаем память
        }
    };

    distributeToThreads(image_min_x + 1, image_max_x - 1, image_min_y - 1, image_max_y - 1, function); // распределяем по потокам
}

void ImageCorrector::aliasing(int radius, int border, int blackBorderPercent, int whiteBorderPercent)
{
    auto imageResultMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения

    auto function = [&](int i, int j)
    {
        if(i % radius == 0 && j % radius == 0)  // Если попадаем в радиус
        {
            int countBiggerBorder = 0;

            for(int k=i-radius; k<i+radius; k++)    // проходим по всем строкам влево и вправо
            {
                for(int g=j-radius; g<j+radius; g++)
                {
                    if(imageResultMatrix[k][g] >= border)   // если значение пикселя больше, чем граница
                    {
                        countBiggerBorder++;
                    }
                }
            }

            double maxPixels = radius * (radius/2);    // считаем максимальное кол-во пикселей
            double blackPercent = (((double)countBiggerBorder) / maxPixels) * 100.0;   // считаем процент пикселей
            double whitePercent = 100.0 - blackPercent; // процент белых пикселей
            if(blackPercent < (double)whiteBorderPercent)   // если кол-во темных пикселей больше чем половина, то весь квадрат заполняем церным цветом
            {
                for(int k=i-radius; k<i+radius; k++)    // проходим по всем строкам влево и вправо
                {
                    for(int g=j-radius; g<j+radius; g++)
                    {
                        imageResultMatrix[k][g] = BLACK_GRAY_LEVEL;
                    }
                }
            }

            if (whitePercent < (double)blackBorderPercent)
            {
                for(int k=i-radius; k<i+radius; k++)    // проходим по всем строкам влево и вправо
                {
                    for(int g=j-radius; g<j+radius; g++)
                    {
                        imageResultMatrix[k][g] = WHITE_GRAY_LEVEL;
                    }
                }
            }
        }
    };

    distributeToThreads(image_min_x + radius, image_max_x - radius, image_min_y + radius, image_max_y - radius, function, radius * 2, radius * 2);
}

void ImageCorrector::medianRadiusFilter(int radius)
{
    auto imageResultMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения

    int i_start = image_min_x + radius;
    int i_end = image_max_x - radius;
    int i_step = 1;

    int j_start = image_min_y + radius;
    int j_end = image_max_y - radius;
    int j_step = 1;

    int **pixelsNewMas = new int* [resultImage.getWidth()];
    for(int i = 0; i<resultImage.getWidth(); i++)
    {
        pixelsNewMas[i] = new int [resultImage.getHeight()];
    }

    distributeToThreads(i_start, i_end, j_start, j_end, [&](int i, int j) // выполняем рассчёт медианных значений для каждого пикселя
    {
        QList<int> pixelsAround;   // массив всех пикселей

        for(int k=i-radius; k<i+radius; k++)    // проходим по всем строкам влево и вправо
        {
            for(int g=j-radius; g<j+radius; g++)
            {
                pixelsAround.append(imageResultMatrix[k][g]);
            }
        }

        std::sort(pixelsAround.begin(), pixelsAround.end());  // сортируем массив (быстрая сортировка)
        int medianValue = pixelsAround.size() / 2;  // получаем номер серединного (медианного) элемента

        pixelsNewMas[i][j] = pixelsAround[medianValue]; // сохраняем полученное значение в новый массив, чтобы не затирать старые значения
    }, i_step, j_step);

    distributeToThreads(i_start, i_end, j_start, j_end, [&](int i, int j)   // заменяем пиксели исходного изображения на получившиеся
    {
        imageResultMatrix[i][j] = pixelsNewMas[i][j];
    });

    for(int i=0; i<resultImage.getWidth(); i++)
    {
        delete [] pixelsNewMas [i];
    }
    delete [] pixelsNewMas;
}

void ImageCorrector::averageFilter(int radius)
{
    auto imageResultMatrix = resultImage.getGrayScaleMatrix();  // матрица пикселей получаемого изображения

    int i_start = image_min_x + radius;
    int i_end = image_max_x - radius;
    int i_step = 1;

    int j_start = image_min_y + radius;
    int j_end = image_max_y - radius;
    int j_step = 1;

    int **pixelsNewMas = new int* [resultImage.getWidth()];
    for(int i = 0; i<resultImage.getWidth(); i++)
    {
        pixelsNewMas[i] = new int [resultImage.getHeight()];
    }

    distributeToThreads(i_start, i_end, j_start, j_end, [&](int i, int j) // выполняем рассчёт средних значений для каждого пикселя
    {
        QList<int> pixelsAround;   // массив всех пикселей

        for(int k=i-radius; k<i+radius; k++)    // проходим по всем строкам влево и вправо
        {
            for(int g=j-radius; g<j+radius; g++)
            {
                pixelsAround.append(imageResultMatrix[k][g]);
            }
        }

        int sum = 0; // сумма значений пикселей
        for(int i=0; i<pixelsAround.size(); i++)
        {
            sum += pixelsAround[i];
        }
        double avg = sum;
        avg /= (double)pixelsAround.size(); // считаем среднее арифметическое
        int value = (int)avg;   // приводим к целому значению для изображения

        pixelsNewMas[i][j] = value; // сохраняем полученное значение в новый массив, чтобы не затирать старые значения
    }, i_step, j_step);

    distributeToThreads(i_start, i_end, j_start, j_end, [&](int i, int j)   // заменяем пиксели исходного изображения на получившиеся
    {
        imageResultMatrix[i][j] = pixelsNewMas[i][j];
    });

    for(int i=0; i<resultImage.getWidth(); i++)
    {
        delete [] pixelsNewMas [i];
    }
    delete [] pixelsNewMas;
}

void ImageCorrector::setPixelColor(QImage &image, int i, int j, QColor color)
{
    if(color.isValid())
    {
        image.setPixelColor(i, j, color); // устанавливаем нвоый цвет
    }
    else
    {
        qDebug() << "Not Valid Color: " << color << color.black();
    }
}

int ImageCorrector::getPixelBlackValue(QImage &image, int i, int j)
{
    return image.pixelColor(i, j).black();
}

QColor ImageCorrector::blackColor(int grayValue)
{
    return QColor(grayValue, grayValue, grayValue);
}

QVector<QVector<bool>> ImageCorrector::getCorrectDataAroundPixels(QImage &image, int i, int j, int border, bool (*compare)(int, int))
{
    QVector<QVector<bool>> matrix;  // матрица соответствия
    matrix.resize(3);   // задаем кол-во строк
    for(auto &&item : matrix)
    {
        item.resize(3); // задаем кол-во столбцов
    }

    auto pixsels = getMatrixAroundPixel(image, i, j);   // получаем матрицу пикселей

    for(int k=0; k<3; k++)
    {
        for(int h=0; h<3; h++)
        {
            int black = pixsels[k][h];  // получаем значение интенсивности пикселя
            bool isCorrect = compare(black, border);  // результат корректности
            matrix[k][h] = isCorrect;   // заносим данные в матрицу
        }
    }
    return matrix;
}

bool **ImageCorrector::getCorrectDataAroundPixels(int **grayScaleMatrix, int i, int j, int border, bool (*compare)(int, int))
{
    bool **correctMatrix = new bool *[3];
    for(int i=0; i<3; i++)
    {
        correctMatrix[i] = new bool [3];
    }

    auto pixsels = getMatrixAroundPixel(grayScaleMatrix, i, j);   // получаем матрицу пикселей

    for(int k=0; k<3; k++)
    {
        for(int h=0; h<3; h++)
        {
            int black = pixsels[k][h];
            bool isCorrect = compare(black, border);  // результат корректности
            correctMatrix[k][h] = isCorrect;    // заносим данные в матрицу
        }
    }

    memoryDoubleArrayFree(pixsels, 3);

    return correctMatrix;
}


QVector<QVector<int> > ImageCorrector::getMatrixAroundPixel(QImage &image, int i, int j)
{
    QVector<QVector<int>> matrix;  // матрица соответствия
    matrix.resize(3);   // задаем кол-во строк
    for(auto &&item : matrix)
    {
        item.resize(3); // задаем кол-во столбцов
    }

    for(int k=0; k<3; k++)
    {
        for(int h=0; h<3; h++)
        {
            // i + (h-1) даст нам значения -1 0 +1
            int black = getPixelBlackValue(image, i + (k-1), j + (h-1));    // берем информацию о пикселях вокруг
            matrix[k][h] = black;   // заносим данные в матрицу
        }
    }
    return matrix;
}

bool **ImageCorrector::getMatrixAroundPixel(int **grayScaleMatrix, int i, int j)
{
    bool **matrix = new bool *[3];
    for(int i=0; i<3; i++)
    {
        matrix[i] = new bool [3];
    }

    for(int k=0; k<3; k++)
    {
        for(int h=0; h<3; h++)
        {
            // i + (h-1) даст нам значения -1 0 +1
            int black = grayScaleMatrix[i + (k-1)][j + (h-1)];  // берем информацию о пикселях вокруг
            matrix[k][h] = black;   // заносим данные в матрицу
        }
    }
    return matrix;
}

void ImageCorrector::setImage_max_y(int newImage_max_y)
{
    image_max_y = newImage_max_y;
}

void ImageCorrector::setImage_max_x(int newImage_max_x)
{
    image_max_x = newImage_max_x;
}

void ImageCorrector::setImage_min_y(int newImage_min_y)
{
    image_min_y = newImage_min_y;
}

void ImageCorrector::setImage_min_x(int newImage_min_x)
{
    image_min_x = newImage_min_x;
}

void ImageCorrector::setFilter(const ImageCorrectrFilterParams &newFilter)
{
    filter = newFilter;
    image_min_x = filter.widthStart;
    image_max_x = filter.widthEnd;
    image_min_y = filter.heightStart;
    image_max_y = filter.heightEnd;
}

void ImageCorrector::distributeToThreads(int startI, int endI, int startJ, int endJ, std::function<void(int, int)> function, int stepI, int stepJ)
{
    int countI = endI - startI; // считаем кол-во значений для i, если начальное значение не 0
    int countJ = endJ - startJ; // считаем кол-во значений для j, если начальное значение не 0

    int threadsCount = filter.threadsCount;

    if(threadsCount <= 1 || countI < threadsCount || countJ < threadsCount)   // если требуется всего 1 поток
    {
        for(int i=startI; i<endI; i += stepI)  // делаем обычный вложенный цикл
        {
            for(int j=startJ; j<endJ; j += stepJ)
            {
                function(i, j); // и выполняем действие, которое нужно сделать
            }
        }
    }
    else    // если требуется несколько потоков
    {
        int intervalI = countI / threadsCount;  // считаем интервал между i для ровного разделения на циклы
        int intervalJ = countJ / threadsCount;  // считаем интервал между j для ровного разделения на циклы

        int *masStartI = new int [threadsCount];    // массив хранения стартовых значений i для каждого потока
        int *masStartJ = new int [threadsCount];    // массив хранения стартовых значений j для каждого потока
        int *masEndI = new int [threadsCount];    // массив хранения конечных значений i для каждого потока
        int *masEndJ = new int [threadsCount];    // массив хранения конечных значений j для каждого потока

        for(int p=0; p<threadsCount; p++)   // заполняем стартовые значения
        {
            masStartI[p] = startI + intervalI * p;  // стартовое значение (константа) + интервал, умноженный на текущую позицию
            masStartJ[p] = startJ + intervalJ * p;  // стартовое значение (константа) + интервал, умноженный на текущую позицию
            masEndI[p] = startI + intervalI * (p+1);    // конечное значение равно следующему стартовому
            masEndJ[p] = startJ + intervalJ * (p+1);    // конечное значение равно следующему стартовому
        }

        masEndI[threadsCount-1] = endI; // последнее значение i равно максимальному
        masEndJ[threadsCount-1] = endJ; // последнее значение j равно максимальному

        std::thread *threads = new std::thread [threadsCount];  // создаём потоки
        for(int k=0; k<threadsCount; k++)   // заполняем действиями потоки
        {
            threads[k] = std::thread([masStartI, masEndI, function, k, startJ, endJ]()    // передаём потоку действие, которое нужно выполнить
            {
                for(int i=masStartI[k]; i<masEndI[k]; i++)  // делаем проход по i
                {
                    for(int j=startJ; j<endJ; j++)  // делаем проход по j
                    {
                        function(i, j); // и выполняем действие, которое нужно сделать
                    }
                }
            });
        }

        for(int i=threadsCount-1; i>=0; i--)    // идём в обратном порядке и дожидаемся завершения всех потоков
        {
            threads[i].join();
        }

        // удалить память !!!
        delete [] threads;
        threads = nullptr;

        delete [] masStartI;
        masStartI = nullptr;

        delete [] masStartJ;
        masStartJ = nullptr;

        delete [] masEndI;
        masEndI = nullptr;

        delete [] masEndJ;
        masEndJ = nullptr;
    }
}

template<typename T>
void ImageCorrector::memoryDoubleArrayFree(T **&array, int count1)
{
    for(int i=0; i<count1; i++)
    {
        delete [] array [i];
        array[i] = nullptr;
    }
    delete [] array;
    array = nullptr;
}

QImage ImageCorrector::getResultImage() const
{
    return resultImage.getImage();
}

void ImageCorrector::setImageObject(const QImage value)
{
    imageObject.setImage(value);
    resultImage.setImage(value);
}

void ImageCorrector::setImageOriginal(const QImage &value)
{
    imageOriginal.setImage(value);
}
