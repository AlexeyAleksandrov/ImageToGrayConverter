#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QImage image = QImage("C:/Users/ASUS/Downloads/cat.jpg");
//    image.convertTo(QImage::Format_Grayscale16);
//    ui->label_image->setPixmap(QPixmap::fromImage(image));
//    ui->label_image->setScaledContents(true);

    QColor color = Qt::red;
    int blackValue = color.black();
    color.setRgb(blackValue, blackValue, blackValue);
    QPalette palette = ui->label_image->palette();
    palette.setColor(QPalette::WindowText, color);
    ui->label_image->setPalette(palette);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_choseInputImage_clicked()
{
    QString imageOriginalDir = ui->lineEdit_imageOriginal->text();
    QString imageObjectDir = ui->lineEdit_imageObject->text();

    if(imageOriginalDir == "")
    {
        QMessageBox::warning(this, "Ошибка", "Выберите изображение оригинала!");
        return;
    }
    if(imageObjectDir == "")
    {
        QMessageBox::warning(this, "Ошибка", "Выберите изображение объекта!");
        return;
    }

    QImage imageOriginal = QImage(imageOriginalDir);
    QImage imageObject = QImage(imageObjectDir);

//    imageOriginal.invertPixels();

    for (int i=0; i<imageOriginal.width(); i++)
    {
        for (int j=0; j<imageOriginal.height(); j++)
        {
            int blackOriginal = imageOriginal.pixelColor(i, j).black();
            int blackObject = imageObject.pixelColor(i, j).black();
            int blackSub = blackObject - blackOriginal;
            imageOriginal.setPixelColor(i, j, QColor(blackSub, blackSub, blackSub));
        }
    }

//    image.convertTo(QImage::Format_Grayscale16);
//    image.invertPixels();

    int widht = ui->label_image->width();
    int height = ui->label_image->height();

    ui->label_image->setMaximumWidth(widht);
    ui->label_image->setMaximumHeight(height);

    ui->label_image->setScaledContents(true);
    ui->label_image->setPixmap(QPixmap::fromImage(imageOriginal));

//    QFile file("pixels.txt");
//    file.open(QIODevice::WriteOnly);
//    for (int i=0; i<image.width(); i++)
//    {
//        for (int j=0; j<image.height(); j++)
//        {
//            QColor color = image.pixel(i, j);
//            file.write(QString(QString::number(color.black()) + "\t").toUtf8());
//        }
//        file.write("\r\n");
//    }
//    file.close();
//    qDebug() << "Готово!";
}

void MainWindow::on_pushButton_choseImageOriginal_clicked()
{
    QString imageDir = QFileDialog::getOpenFileName(this, "Выбор картинки");
    ui->lineEdit_imageOriginal->setText(imageDir);
}


void MainWindow::on_pushButton_choseImageObject_clicked()
{
    QString imageDir = QFileDialog::getOpenFileName(this, "Выбор картинки");
    ui->lineEdit_imageObject->setText(imageDir);
}


void MainWindow::on_pushButton_calculate_clicked()
{
    QString imageOriginalDir = ui->lineEdit_imageOriginal->text();
    QString imageObjectDir = ui->lineEdit_imageObject->text();

    if(imageOriginalDir == "")
    {
        QMessageBox::warning(this, "Ошибка", "Выберите изображение оригинала!");
        return;
    }
    if(imageObjectDir == "")
    {
        QMessageBox::warning(this, "Ошибка", "Выберите изображение объекта!");
        return;
    }

    QImage imageOriginal = QImage(imageOriginalDir);
    QImage imageObject = QImage(imageObjectDir);

    imageOriginal.convertTo(QImage::Format_Grayscale16);
    imageObject.convertTo(QImage::Format_Grayscale16);

    resultImage = imageOriginal;

//    imageOriginal.invertPixels();

    for (int i=0; i<imageOriginal.width(); i++)
    {
        for (int j=0; j<imageOriginal.height(); j++)
        {
            int blackOriginal = imageOriginal.pixelColor(i, j).black();
            int blackObject = imageObject.pixelColor(i, j).black();
            int blackSub = abs(blackOriginal - blackObject);
            QColor color = QColor(blackSub, blackSub, blackSub);
            if(color.isValid())
            {
                resultImage.setPixelColor(i, j, color);
            }
            else
            {
                qDebug() << "Not Valid Color: " << color << blackSub;
            }
        }
    }

    resultImage.invertPixels();

//    image.convertTo(QImage::Format_Grayscale16);
//    image.invertPixels();

    int widht = ui->label_image->width();
    int height = ui->label_image->height();

    ui->label_image->setMaximumWidth(widht);
    ui->label_image->setMaximumHeight(height);

    ui->label_image->setScaledContents(true);
    ui->label_image->setPixmap(QPixmap::fromImage(resultImage));

    ui->radioButton_result->setChecked(true);

}


void MainWindow::on_radioButton_original_clicked()
{
    QString imageDir = ui->lineEdit_imageOriginal->text();
    if(imageDir != "")
    {
        ui->label_image->setPixmap(QPixmap::fromImage(QImage(imageDir)));
    }
}


void MainWindow::on_radioButton_object_clicked()
{
    QString imageDir = ui->lineEdit_imageObject->text();
    if(imageDir != "")
    {
        ui->label_image->setPixmap(QPixmap::fromImage(QImage(imageDir)));
    }
}


void MainWindow::on_radioButton_result_clicked()
{
    ui->label_image->setPixmap(QPixmap::fromImage(QImage(resultImage)));
}


void MainWindow::on_pushButton_saveResult_clicked()
{
    QString saveDir = QFileDialog::getSaveFileName(this, "Сохранить как", "", "Images (*.jpg)");
    if(saveDir != "")
    {
        resultImage.save(saveDir);
    }
}

