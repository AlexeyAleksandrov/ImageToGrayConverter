#include "imagedataexception.h"


ImageDataException::ImageDataException(QString errorText)
{
    this->text = errorText;
}

const char *ImageDataException::what() const throw()
{
    const std::string *er = new std::string(QString("ImageDataException: " + text).toStdString());
    const char *ch = er->c_str();
    return ch;
}
