#include "imagecorrectrfilterparams.h"

ImageCorrectrFilterParams::ImageCorrectrFilterParams()
{

}

QJsonObject ImageCorrectrFilterParams::getFilterJsonObject()
{
    QJsonObject object; // объект JSON, в который запишутся данные

    object["filterName"] = filterName;
    qDebug() << "getFilterJsonObject " << filterName << object["filterName"].toString();

    // параметры включения и выключения фильтров
    object["needSubstructImage"] = needSubstructImage;
    object["needHardDeleteNoise"] = needHardDeleteNoise;
    object["needHardDeleteWhiteColor"] = needHardDeleteWhiteColor;
    object["needHardDeleteBlackColor"] = needHardDeleteBlackColor;
    object["needMedianFilter"] = needMedianFilter;
    object["needAverageFilter"] = needAverageFilter;
    object["needAliasing"] = needAliasing;
    object["needColorInversion"] = needColorInversion;
    object["needAliasingVisualisation"] = needAliasingVisualisation;

    // параметры диапазона выборки значений
    object["widthStart"] = widthStart;
    object["heightStart"] = heightStart;
    object["widthEnd"] = widthEnd;
    object["heightEnd"] = heightEnd;

    // системные параметры
    object["threadsCount"] = threadsCount;
    object["repeatOffset"] = repeatOffset;

    // параметры фильтров
    object["clippingNoiseValue"] = clippingNoiseValue;
    object["blackEnchancement"] = blackEnchancement;

    object["hardDeleteNoiseBorder"] = hardDeleteNoiseBorder;
    object["hardDeleteNoiseDeleteType"] = hardDeleteNoiseDeleteType;
    object["medianFilter_radius"] = medianFilter_radius;
    object["averageFilter_radius"] = averageFilter_radius;
    object["aliasingRadius"] = aliasingRadius;
    object["aliasingBorder"] = aliasingBorder;
    object["aliasingBlackBorder"] = aliasingBlackBorder;
    object["aliasingWhiteBorder"] = aliasingWhiteBorder;

    return object;
}

void ImageCorrectrFilterParams::setFilterParamsFromJsonObject(QJsonObject object)
{
    filterName = object["filterName"].toString();

    // параметры включения и выключения фильтров
    needSubstructImage = object["needSubstructImage"].toBool();
    needHardDeleteNoise = object["needHardDeleteNoise"].toBool();
    needHardDeleteWhiteColor = object["needHardDeleteWhiteColor"].toBool();
    needHardDeleteBlackColor = object["needHardDeleteBlackColor"].toBool();
    needMedianFilter = object["needMedianFilter"].toBool();
    needAverageFilter = object["needAverageFilter"].toBool();
    needAliasing = object["needAliasing"].toBool();
    needColorInversion = object["needColorInversion"].toBool();
    needAliasingVisualisation = object["needAliasingVisualisation"].toBool();

    // параметры диапазона выборки значений
    widthStart = object["widthStart"].toInt();
    heightStart = object["heightStart"].toInt();
    widthEnd = object["widthEnd"].toInt();
    heightEnd = object["heightEnd"].toInt();

    // системные параметры
    threadsCount = object["threadsCount"].toInt();
    repeatOffset = object["repeatOffset"].toInt();

    // параметры фильтров
    clippingNoiseValue = object["clippingNoiseValue"].toInt();
    blackEnchancement = object["blackEnchancement"].toInt();

    hardDeleteNoiseBorder = object["hardDeleteNoiseBorder"].toInt();
    hardDeleteNoiseDeleteType = ImageCorrectorEnums::NoiseDeleteTypes(object["hardDeleteNoiseDeleteType"].toInt());
    medianFilter_radius = object["medianFilter_radius"].toInt();
    averageFilter_radius = object["averageFilter_radius"].toInt();
    aliasingRadius = object["aliasingRadius"].toInt();
    aliasingBorder = object["aliasingBorder"].toInt();
    aliasingBlackBorder = object["aliasingBlackBorder"].toInt();
    aliasingWhiteBorder = object["aliasingWhiteBorder"].toInt();
}
