#ifndef IMAGECORRECTORENUMS_H
#define IMAGECORRECTORENUMS_H


class ImageCorrectorEnums
{
public:
    ImageCorrectorEnums();

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
};

#endif // IMAGECORRECTORENUMS_H
