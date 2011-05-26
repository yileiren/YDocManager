#-------------------------------------------------
#
# Project created by QtCreator 2011-05-25T14:58:31
#
#-------------------------------------------------

QT       += core gui

TARGET = YDocManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    yricheditor.h

FORMS    += mainwindow.ui

win32{
    debug{
        LIBS += D:/MyProject/YQtDesignerPlugin-build-desktop/YRichEditor/debug/libYRichEditorPlugin.a
    }

    release{
        LIBS += D:/MyProject/YQtDesignerPlugin-build-desktop/YRichEditor/release/libYRichEditorPlugin.a
    }
}

linux-g++{
    LIBS += /home/yileiren/code/YDocManager/YRichEditor/linux/debug/yricheditor.o
}

OTHER_FILES += \
    YRichEditor/win32/debug/YRichEditorPlugin.dll \
    YRichEditor/win32/debug/libYRichEditorPlugin.a \
    YRichEditor/win32/release/YRichEditorPlugin.dll \
    YRichEditor/win32/release/libYRichEditorPlugin.a \
    images/icos/app.png \
    myapp.rc \
    YRichEditor/linux/libYRichEditorPlugin.so \
    YRichEditor/linux/debug/yricheditor.o \
    YRichEditor/linux/release/yricheditor.o \
    images/icos/app.ico \
    images/menu/file/save.png \
    images/menu/file/open.png \
    images/menu/file/new.png \
    images/menu/file/exit.png \
    images/menu/file/edit.png \
    images/menu/file/close.png

RC_FILE += myapp.rc

RESOURCES += \
    images.qrc
