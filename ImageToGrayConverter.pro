QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
#    src/FiltersManager/FiltersEntities/clipnoisefilter.cpp \
#    src/FiltersManager/FiltersEntities/enchanceblackcolorfilter.cpp \
#    src/FiltersManager/FiltersEntities/hardclipnoisefilter.cpp \
#    src/FiltersManager/FiltersEntities/substructimagefilter.cpp \
#    src/FiltersManager/FiltersInterfaces/icheckfilter.cpp \
#    src/FiltersManager/FiltersInterfaces/icombofilter.cpp \
#    src/FiltersManager/FiltersInterfaces/isliderfilter.cpp \
#    src/FiltersManager/filtersmanager.cpp \
#    src/FiltersManager/ifilter.cpp \
    src/ImageHandlers/imagecorrector.cpp \
    src/ImageHandlers/imagedata.cpp \
    src/ImageHandlers/imagedataexception.cpp \
    src/ImageHandlers/imagetransformer.cpp \
    src/MainWindow/mainwindow.cpp \
    src/UiHandlers/uidatasaver.cpp \
    src/main.cpp


HEADERS += \
#    src/FiltersManager/FiltersEntities/clipnoisefilter.h \
#    src/FiltersManager/FiltersEntities/enchanceblackcolorfilter.h \
#    src/FiltersManager/FiltersEntities/hardclipnoisefilter.h \
#    src/FiltersManager/FiltersEntities/substructimagefilter.h \
#    src/FiltersManager/FiltersInterfaces/icheckfilter.h \
#    src/FiltersManager/FiltersInterfaces/icombofilter.h \
#    src/FiltersManager/FiltersInterfaces/isliderfilter.h \
#    src/FiltersManager/filtersmanager.h \
#    src/FiltersManager/ifilter.h \
    src/ImageHandlers/imagecorrector.h \
    src/ImageHandlers/imagedata.h \
    src/ImageHandlers/imagedataexception.h \
    src/ImageHandlers/imagetransformer.h \
    src/MainWindow/mainwindow.h \
    src/UiHandlers/uidatasaver.h


FORMS += \
    src/MainWindow/mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
