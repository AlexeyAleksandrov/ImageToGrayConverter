QT       += core gui multimedia multimediawidgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/FiltersManager/filter.cpp \
    src/FiltersManager/filtersmanager.cpp \
    src/ImageHandlers/imagecorrector.cpp \
    src/ImageHandlers/imagedata.cpp \
    src/ImageHandlers/imagedataexception.cpp \
    src/ImageHandlers/imagetransformer.cpp \
    src/MainWindow/mainwindow.cpp \
    src/UiHandlers/uidatasaver.cpp \
    src/filtersmanager.cpp \
    src/main.cpp


HEADERS += \
    src/FiltersManager/filter.h \
    src/FiltersManager/filtersmanager.h \
    src/ImageHandlers/imagecorrector.h \
    src/ImageHandlers/imagedata.h \
    src/ImageHandlers/imagedataexception.h \
    src/ImageHandlers/imagetransformer.h \
    src/MainWindow/mainwindow.h \
    src/UiHandlers/uidatasaver.h \
    src/filtersmanager.h


FORMS += \
    src/MainWindow/mainwindow.ui


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
