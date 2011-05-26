#include <QtGui/QApplication>
#include "mainwindow.h"
#include "forms/appopen.h"
#include "ydocsyetem.h"

#include <QDialog>
#include <QTextCodec>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //初始化系统信息
    SystemInfo *systemInfo = new SystemInfo;
    systemInfo->documentPath = QObject::tr("");
    systemInfo->progectFile = QObject::tr("");
    systemInfo->version = QObject::tr("");

    //设置编码集格式
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));

    //设置语言
    QTranslator tra;
    tra.load(":/languages/zh_CN.qm");
    a.installTranslator(&tra);

    //系统初始化窗口
    AppOpen appOpen;
    appOpen.systemInfo = systemInfo;
    appOpen.setWindowFlags(Qt::FramelessWindowHint);
    int returnCode = appOpen.exec();


    //程序主窗口
    MainWindow w;
    if(QDialog::Accepted == returnCode)
    {
        w.show();
    }
    else
    {
        return 0;
    }
    return a.exec();
}
