#include "appopen.h"
#include "ui_appopen.h"

#include <QTimer>
#include <QMessageBox>
#include <QTextStream>
#include <QXmlStreamReader>
#include <QDesktopWidget>

AppOpen::AppOpen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::appOpen),
    isShown(false),
    initState(AppOpen::Ready), //初始化状态设置成默认（就绪）
    infoFile(tr(SYSTEM_INFO_FILE)) //初始化系统信息文件
{
    ui->setupUi(this);

    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(initSystem()));

    //设置窗口居中显示
    QDesktopWidget * desktop = QApplication::desktop();
    this->move((desktop->width() - this->width()) / 2,(desktop->height() - this->height()) / 2);
}

AppOpen::~AppOpen()
{
    delete ui;
    delete this->timer;
}

void AppOpen::showEvent(QShowEvent *e)
{
    if(!this->isShown)
    {
        this->isShown = true;

        //启动定时器
        timer->start(500);
    }
}

void AppOpen::initSystem()
{
    //关闭计时器
    this->timer->stop();

    bool isExists = this->infoFile.exists();
    if(isExists)
    {
        this->ui->initText->setText(tr("读取系统信息..."));
        delete this->timer;
        this->timer = new QTimer(this);
        connect(this->timer, SIGNAL(timeout()), this, SLOT(readSystemInfo()));
        this->timer->start(500);
    }
    else
    {
        this->ui->initText->setText(tr("创建默认系统信息..."));
        delete this->timer;
        this->timer = new QTimer(this);
        connect(this->timer, SIGNAL(timeout()), this, SLOT(createInfoFile()));
        this->timer->start(500);
    }
}

void AppOpen::createInfoFile()
{
    this->timer->stop();
    //创建新的系统信息文件
    if(!this->infoFile.open(QFile::WriteOnly|QFile::Truncate))
    {
        this->initState = NoInfoFile;
        this->reject();
    }
    QTextStream ts(&this->infoFile);
    ts.setCodec("utf-8");

    //写入默认系统信息
    ts << tr(DEFINE_SYSTEM_INFO_TEXT);
    this->infoFile.close();

    this->ui->initText->setText(tr("读取系统信息..."));
    delete this->timer;
    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(readSystemInfo()));
    this->timer->start(500);
}

void AppOpen::readSystemInfo()
{
    this->timer->stop();
    //打开文件
    if(this->infoFile.open(QFile::ReadOnly))
    {
        //读取系统信息
        QXmlStreamReader xmlReader;
        xmlReader.setDevice(&this->infoFile);
        while(!xmlReader.atEnd())
        {
            if(xmlReader.isStartElement())
            {
                QString name = xmlReader.name().toString();
                if(name == SYSTEM_VERSION_TAG)
                {
                    this->systemInfo->version = xmlReader.attributes().value(tr("value")).toString();
                }
                else if(name == SYSTEM_DOCUMENT_TAG)
                {
                    this->systemInfo->documentPath = xmlReader.attributes().value(tr("value")).toString();
                }
                else if(name == SYSTEM_PROGECT_FILE_TAG)
                {
                    this->systemInfo->progectFile = xmlReader.attributes().value(tr("value")).toString();
                }
                xmlReader.readNext();
            }
            else
            {
                xmlReader.readNext();
            }
        }
        this->infoFile.close();

        //检查系统信息是否齐全
        if(this->systemInfo->documentPath == tr("") || //没有文档路径
                this->systemInfo->progectFile == tr("") ||  //没有工程文件
                this->systemInfo->version == tr("")) //没有版本信息
        {
            this->ui->initText->setText(tr("创建默认系统信息..."));
            delete this->timer;
            this->timer = new QTimer(this);
            connect(this->timer, SIGNAL(timeout()), this, SLOT(createInfoFile()));
            this->timer->start(500);
        }
        else
        {
            //检查文档存储路径
            QDir docDir;
            bool isDirExitsts = docDir.exists(this->systemInfo->documentPath);
            if(!isDirExitsts)
            {
                //创建路径
                bool isSuccessful = docDir.mkdir(this->systemInfo->documentPath);
                if(!isSuccessful)
                {
                    //创建路径失败
                    this->initState = CreateDirError;
                    this->reject();
                }
            }

            //检查HTML文档存储文件夹
            isDirExitsts = docDir.exists(this->systemInfo->documentPath + tr("/") + HTML_DOC_PATH);
            if(!isDirExitsts)
            {
                //创建路径
                bool isSuccessful = docDir.mkdir(this->systemInfo->documentPath + tr("/") + HTML_DOC_PATH);
                if(!isSuccessful)
                {
                    //创建路径失败
                    this->initState = CreateDirError;
                    this->reject();
                }
            }

            //检查TEXT文档存储文件夹
            isDirExitsts = docDir.exists(this->systemInfo->documentPath + tr("/") + TEXT_DOC_PATH);
            if(!isDirExitsts)
            {
                //创建路径
                bool isSuccessful = docDir.mkdir(this->systemInfo->documentPath + tr("/") + TEXT_DOC_PATH);
                if(!isSuccessful)
                {
                    //创建路径失败
                    this->initState = CreateDirError;
                    this->reject();
                }
            }

            this->ui->initText->setText(tr("检查文档信息..."));
            delete this->timer;
            this->timer = new QTimer(this);
            connect(this->timer, SIGNAL(timeout()), this, SLOT(checkDocInfo()));
            this->timer->start(500);
        }
    }
    else
    {
        this->initState = ReadInfoFileError;
        this->reject();
    }
}

void AppOpen::checkDocInfo()
{
    this->timer->stop();
    //检查文档信息
    QFile docInfoFile;
    docInfoFile.setFileName(this->systemInfo->progectFile);

    if(docInfoFile.open(QFile::ReadOnly))
    {
        //读取文档信息
        QXmlStreamReader xmlReader;
        xmlReader.setDevice(&docInfoFile);
        while(!xmlReader.atEnd())
        {
            if(xmlReader.isStartElement())
            {
                QString name = xmlReader.name().toString();
                if(name == DOC_UUID_TAG)
                {
                    this->docInfo->uuid = xmlReader.attributes().value(tr("value")).toString();
                }
                else if(name == DOC_NAME_TAG)
                {
                    this->docInfo->name = xmlReader.attributes().value(tr("value")).toString();
                }
                else if(name == DOC_NUM_TAG)
                {
                    this->docInfo->num = xmlReader.attributes().value(tr("value")).toString();
                }
                else if(name == DOC_USER_NAME_TAG)
                {
                    this->docInfo->userName = xmlReader.attributes().value(tr("value")).toString();
                }
                else if(name == DOC_USER_NUM_TAG)
                {
                    this->docInfo->userNum = xmlReader.attributes().value(tr("value")).toString();
                }
                else if(name == DOC_CREATE_TIME_TAG)
                {
                    this->docInfo->createTime = QDateTime::fromString(xmlReader.attributes().value(tr("value")).toString(),DATE_TIME_FORMAT);
                }
                else if(name == DOC_ROOT_XML_TAG)
                {
                    this->docInfo->rootXML = xmlReader.attributes().value(tr("value")).toString();
                }

                xmlReader.readNext();
            }
            else
            {
                xmlReader.readNext();
            }
        }
        docInfoFile.close();
        docInfoFile.close();

        //检查文档信息是否齐全
        if(this->docInfo->name == tr("") || //文档名不存在
                this->docInfo->uuid == tr("") || //uuid不存在
                this->docInfo->num == tr("") || //文档编号不存在
                this->docInfo->userName == tr("") || //用户名不存在
                this->docInfo->userNum == tr("") || //用户编号不存在
                this->docInfo->rootXML == tr("") || //文档根XML不存在
                this->docInfo->createTime == QDateTime::fromString(DOC_DEFINE_TIME,DATE_TIME_FORMAT)) //创建时间不存在
        {
            //文档信息有误
            this->initState = DocInfoError;
            this->reject();
        }
        else
        {
            this->accept();
        }
    }
    else
    {
        this->initState = NoDocInfoFile;
        this->reject();
    }
}
