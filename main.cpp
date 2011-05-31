#include <QtGui/QApplication>
#include "mainwindow.h"
#include "forms/appopen.h"
#include "ydocsyetem.h"
#include "forms/docinfoedit.h"
#include "appXML/docinfoxml.h"

#include <QDialog>
#include <QTextCodec>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //初始化系统信息
    SystemInfo systemInfo;
    systemInfo.documentPath = QObject::tr("");
    systemInfo.progectFile = QObject::tr("");
    systemInfo.version = QObject::tr("");

    //初始化文档信息
    DocInfo docInfo;
    docInfo.createTime = QDateTime::fromString(QObject::tr(DOC_DEFINE_TIME),QObject::tr(DATE_TIME_FORMAT));
    docInfo.name = QObject::tr("");
    docInfo.num = QObject::tr("");
    docInfo.rootXML = QObject::tr("");
    docInfo.userName = QObject::tr("");
    docInfo.userNum = QObject::tr("");
    docInfo.uuid = QObject::tr("");

    //设置编码集格式
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF8"));

    //设置语言
    QTranslator tra;
    tra.load(":/languages/zh_CN.qm");
    a.installTranslator(&tra);

    //系统初始化窗口
    AppOpen appOpen;
    appOpen.systemInfo = &systemInfo;
    appOpen.docInfo = &docInfo;
    appOpen.setWindowFlags(Qt::FramelessWindowHint);
    int returnCode = appOpen.exec();

    //程序主窗口
    MainWindow w;
    if(QDialog::Accepted == returnCode)
    {
        w.docInfo = &docInfo;
        w.systemInfo = &systemInfo;
        w.show();
    }
    else
    {
        if(AppOpen::NoDocInfoFile == appOpen.initState || //没找到文档信息
                AppOpen::DocInfoError == appOpen.initState) //文档信息有错误
        {
            //编辑文档信息
            DocInfoEdit docInfoEdit;
            docInfoEdit.docInfo = &docInfo;
            returnCode = docInfoEdit.exec();

            if(QDialog::Accepted == returnCode)
            {
                //创建文档信息
                if(DocInfoXML::createDocInfo(&docInfo,systemInfo.progectFile))
                {
                    w.docInfo = &docInfo;
                    w.systemInfo = &systemInfo;
                    w.show();
                }
                else
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    return a.exec();
}
