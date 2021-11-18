#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <qdebug.h>
#include <QRgb>
#include <QMessageBox>
#include "uidatasaver.h"
#include "imagecorrector.h"
#include "imagetransformer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_choseImageOriginal_clicked();

    void on_pushButton_choseImageObject_clicked();

    void on_pushButton_calculate_clicked();

    void on_radioButton_original_clicked();

    void on_radioButton_object_clicked();

    void on_radioButton_result_clicked();

    void on_pushButton_saveResult_clicked();

    void on_horizontalSlider_clippingNoiseValue_valueChanged(int value);

    void on_horizontalSlider_blackEnchancementValue_valueChanged(int value);

    void on_horizontalSlider_deleteNoise_valueChanged(int value);

private:
    void setImageToOutputLabel(QImage image);  // вывести картинку

private:
    Ui::MainWindow *ui;

    QImage imageOriginal;
    QImage imageObject;
    QImage resultImage;

    UiDataSaver uiDataSaver;
};
#endif // MAINWINDOW_H
