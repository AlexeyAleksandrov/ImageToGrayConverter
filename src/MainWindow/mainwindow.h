#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <qdebug.h>
#include <QRgb>
#include <QMessageBox>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QCameraImageCapture>
#include <QCloseEvent>
#include <QTime>
#include <QTableWidgetItem>
#include <QResizeEvent>


#include "src/UiHandlers/uidatasaver.h"
#include "src/ImageHandlers/imagecorrector.h"
#include "src/ImageHandlers/imagetransformer.h"
#include "src/FiltersManager/filtersmanager.h"
#include "src/FiltersManager/FiltersEntities/clipnoisefilter.h"
#include "src/FiltersManager/FiltersEntities/enchanceblackcolorfilter.h"
#include "src/FiltersManager/FiltersEntities/hardclipnoisefilter.h"
#include "src/FiltersManager/FiltersEntities/substructimagefilter.h"

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

    void on_pushButton_presets_accept_clicked();

    void on_pushButton_presets_addNew_clicked();

    void on_pushButton_presets_updateCurrent_clicked();

    void on_pushButton_presets_deleteChosed_clicked();

    void on_pushButton_screen_clicked();

    void on_radioButton_imageEmitter_imageFromFile_clicked();

    void on_radioButton_imageEmitter_videoCaptureFromScreen_clicked();

    void on_pushButton_runVideo_clicked();

    void on_pushButton_choseImageObject_video_clicked();

public:
    void setImageToOutputLabel(QImage image);  // вывести картинку
    void updateLabelImageSize();    // перерасчитать азмеры label с изображением
    void processImageFilters(QImage &imageOriginal, QImage &imageObject, QImage &resultImage);

private:
    // экраны
    QStringList getScreensList();   // получить список мониторов

    // камера
    QStringList getCamerasList();   // получить список камер, подключенных к ПК

    QCamera *camera = nullptr;    // объект камеры
    QCameraViewfinder *viewfinder = nullptr;  // обработчик камеры
    QCameraImageCapture *imageCapture = nullptr;  // обработчик снимков камеры
    QList<QCameraInfo> cameras; // список доступных камер
//    int *gmas = nullptr;
//    QTime frameTime;
    FiltersManager filtersManager;  // управление фильтрами

private slots:
    void cameraImageCaptured(int id, const QImage &preview);    // получаение изображения
    void cameraReadyForCaptureChanged(bool ready); // готовы дальше получать изображения

    void on_radioButton_captureDevice_monitor_clicked();

    void on_radioButton_captureDevice_camera_clicked();

    void on_pushButton_choseScreen_clicked();

    void on_checkBox_deleteNoise_stateChanged(int arg1);

    void on_radioButton_imageEmitter_videoCaptureFromScreen_clicked(bool checked);

    void on_radioButton_imageEmitter_imageFromFile_clicked(bool checked);

private:
    Ui::MainWindow *ui;

    QImage imageOriginal;
    QImage imageObject;
    QImage resultImage;

    QLabel *imageLabel= nullptr;    // label, в который будет выводиться изображение

    ImageCorrector imageCorrector;   // обработчик изображений

    UiDataSaver uiDataSaver;

    QScreen *screen = nullptr;  // экран, с которого мы будем брать изображение
    bool isRunning = false;
    bool isScreening = false;

    void closeEvent(QCloseEvent *event)
    {
        qDebug() << "close";
        event->accept();
        this->close();
    };

    void resizeEvent(QResizeEvent *event)
    {
//        qDebug() << "resize";
        updateLabelImageSize();
        event->accept();
    };
};
#endif // MAINWINDOW_H
