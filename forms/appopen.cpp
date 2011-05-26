#include "appopen.h"
#include "ui_appopen.h"
#include "ydocsyetem.h"

#include <QTimer>
#include <QFile>

AppOpen::AppOpen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::appOpen),
    isShown(false)
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
        timer->start(1000);
    }
}

void AppOpen::initSystem()
{
    //关闭计时器
    this->timer->stop();

    //判断系统信息文件是否存在
    this->ui->initText->setText(tr("检查系统信息..."));
    QFile file;
    file.setFileName(tr(SYSTEM_INFO));
    bool isExists = file.exists();
    if(isExists)
    {
        this->ui->initText->setText(tr("找到系统信息..."));
        this->accept();
    }
    else
    {
        this->ui->initText->setText(tr("系统未初始化..."));
        this->reject();
    }
}
