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
