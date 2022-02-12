#ifndef IMAGEDATAEXCEPTION_H
#define IMAGEDATAEXCEPTION_H

#include <QObject>

// ошибка данных для ImageData

class ImageDataException :  public std::exception
{
private:
    QString text;

public:
    ImageDataException(QString errorText);

    const char *what() const throw();

};

#endif // IMAGEDATAEXCEPTION_H
