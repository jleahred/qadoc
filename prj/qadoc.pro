#-------------------------------------------------
#
# Project created by QtCreator 2010-11-14T11:23:25
#
#-------------------------------------------------



QT += webkit
QT       += core gui

QMAKE_CXXFLAGS += -std=gnu++0x


TARGET = qadoc
TEMPLATE = app


SOURCES += ../src/main.cpp\
        ../src/mainwindow.cpp \
        ../src/highlighter.cpp \
        ../src/forms/dialog_compile_options.cpp \
    ../extlibs/mqeditor/src/mqeditor.cpp

HEADERS  += ../src/mainwindow.h \
    ../src/highlighter.h \
    ../src/forms/dialog_compile_options.h  \
    ../extlibs/mqeditor/src/mqeditor.h

FORMS += \
    ../src/forms/dialog_compile_options.ui


INCLUDEPATH += ../extlibs/mqeditor/src  ../extlibs/maiquel-toolkit-cpp/src

LIBS = -laspell -L../extlibs/maiquel-toolkit-cpp/lib       -lmtksupport  -lyaml


RESOURCES += \
    ../src/rc.qrc
