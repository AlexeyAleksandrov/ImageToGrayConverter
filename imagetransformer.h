#ifndef IMAGETRANSFORMER_H
#define IMAGETRANSFORMER_H

#include <QObject>
#include <QImage>
#include <QPixmap>
#include <QLabel>

class ImageTransformer : public QObject
{
    Q_OBJECT
public:
    explicit ImageTransformer(QObject *parent = nullptr);

    static QImage getTransformedImage(QImage source, int out_x, int heigh); // возвращает картинку приведённую к нужному размеру
    static QImage getScaledImage(QImage source, double scalefactor); // возвращает картинку увеличенную в масштабе

signals:

};

#endif // IMAGETRANSFORMER_H
