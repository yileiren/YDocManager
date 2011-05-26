#include "appopen.h"
#include "ui_appopen.h"

#include <QTimer>
#include <QMessageBox>
#include <QTextStream>
#include <QXmlStreamReader>

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
}

AppOpen::~AppOpen()
{
    delete ui;
    delete this->timer;
}

void AppOpen::paintEvent(QPaintEvent *e)
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
}

void AppOpen::readSystemInfo()
{
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
                    this->systemInfo->version = xmlReader.attributes().value("value").toString();
                }
                else if(name == SYSTEM_DOCUMENT_TAG)
                {
                    this->systemInfo->documentPath = xmlReader.attributes().value("value").toString();
                }
                else if(name == SYSTEM_PROGECT_FILE_TAG)
                {
                    this->systemInfo->progectFile = xmlReader.attributes().value("value").toString();
                }
                xmlReader.readNext();
            }
            else
            {
                xmlReader.readNext();
            }
        }
        this->infoFile.close();
        this->accept();
    }
    else
    {
        this->initState = ReadInfoFileError;
        this->reject();
    }
}
