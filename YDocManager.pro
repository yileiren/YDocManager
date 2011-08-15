#-------------------------------------------------
#
# Project created by QtCreator 2011-05-25T14:58:31
#
#-------------------------------------------------

QT       += core gui

TARGET = YDocManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    forms/appopen.cpp \
    forms/docinfoedit.cpp \
    appXML/docinfoxml.cpp \
    forms/editfileinfo.cpp \
    forms/choseimage.cpp \
    forms/inserttable.cpp

HEADERS  += mainwindow.h \
    yricheditor.h \
    forms/appopen.h \
    ydocsyetem.h \
    forms/docinfoedit.h \
    appXML/docinfoxml.h \
    forms/editfileinfo.h \
    forms/choseimage.h \
    forms/inserttable.h

FORMS    += mainwindow.ui \
    forms/appopen.ui \
    forms/docinfoedit.ui \
    forms/editfileinfo.ui \
    forms/choseimage.ui \
    forms/inserttable.ui

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
    images/menu/file/close.png \
    systemInfo.xml \
    languages/qt_zh_CN.qm \
    images/appWindow/initWindowBackImage.png \
    images/tree/doc.png \
    images/tree/dir.png \
    images/logo.gif \
    images/appWindow/initWindowBackImage.psd \
    images/textEdit/textunder.png \
    images/textEdit/textright.png \
    images/textEdit/textleft.png \
    images/textEdit/textitalic.png \
    images/textEdit/textcenter.png \
    images/textEdit/textbold.png \
    images/textEdit/fileprint.png \
    images/textEdit/editundo.png \
    images/textEdit/editredo.png \
    images/textEdit/FontsColor.png \
    images/textEdit/backgroundColor.png \
    images/textEdit/lists/8.png \
    images/textEdit/lists/7.png \
    images/textEdit/lists/6.png \
    images/textEdit/lists/5.png \
    images/textEdit/lists/4.png \
    images/textEdit/lists/3.png \
    images/textEdit/lists/2.png \
    images/textEdit/lists/1.png \
    images/textEdit/table/table.png \
    images/textEdit/insertImage.png

RC_FILE += myapp.rc

RESOURCES += \
    images.qrc \
    languages.qrc
