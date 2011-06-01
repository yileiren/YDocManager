#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "appXML/docinfoxml.h"

#include <QDesktopWidget>
#include <QFile>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isShow(false),
    rootItem(NULL)
{
    ui->setupUi(this);

    //设置窗口居中显示
    QDesktopWidget * desktop = QApplication::desktop();
    this->move((desktop->width() - this->width()) / 2,(desktop->height() - this->height()) / 2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readChildItem(QTreeWidgetItem *parent)
{
    if(FileInfo::dir == parent->data(1,0).value<FileInfo *>()->fileType)
    {
        this->statusLabel.setText(tr("读取文件信息..."));
        //获取路径
        FileInfo *fileInfo = parent->data(1,0).value<FileInfo *>();
        std::vector<FileInfo> filesInfo = DocInfoXML::readFilesInfo(fileInfo->name  + tr("/") +
                                                                    tr(DEFINE_ROOT_DOC_XML_NAME) + tr(DEFINE_DOC_INFO_XML_EXPANDED_NAME));

        //循环设置节点和读取下级节点
        for(std::vector<FileInfo>::size_type s = 0;s < filesInfo.size();s++)
        {
            QTreeWidgetItem * item = new QTreeWidgetItem(parent);
            item->setText(0,filesInfo[s].title);
            if(FileInfo::dir == filesInfo[s].fileType)
            {
                item->setIcon(0,QIcon(tr(":/images/tree/dir.png")));
            }
            else if(FileInfo::doc == filesInfo[s].fileType)
            {
                item->setIcon(0,QIcon(tr(":/images/tree/doc.png")));
            }
            item->setData(1,0,QVariant::fromValue(&filesInfo[s]));

        }
    }
}

bool MainWindow::writeDocInfoXML(QTreeWidgetItem *item)
{
    //获取文件信息
    if(FileInfo::dir == item->data(1,0).value<FileInfo *>()->fileType)
    {
        this->statusLabel.setText(tr("写入文件信息..."));
        //判断文件夹是否存在
        QDir itemDir;
        bool isDirExitsts = itemDir.exists(item->data(1,0).value<FileInfo *>()->name);
        if(!isDirExitsts)
        {
            this->statusLabel.setText(tr("创建文档路径..."));
            //创建路径
            bool isSuccessful = itemDir.mkdir(item->data(1,0).value<FileInfo *>()->name);
            if(!isSuccessful)
            {
                //创建路径失败
                return false;
            }
        }

        //检查HTML文档存储文件夹
        isDirExitsts = itemDir.exists(item->data(1,0).value<FileInfo *>()->name + tr("/") + HTML_DOC_PATH);
        if(!isDirExitsts)
        {
            this->statusLabel.setText(tr("创建HTML路径..."));
            //创建路径
            bool isSuccessful = itemDir.mkdir(item->data(1,0).value<FileInfo *>()->name + tr("/") + HTML_DOC_PATH);
            if(!isSuccessful)
            {
                //创建路径失败
                return false;
            }
        }

        //检查TEXT文档存储文件夹
        isDirExitsts = itemDir.exists(item->data(1,0).value<FileInfo *>()->name + tr("/") + TEXT_DOC_PATH);
        if(!isDirExitsts)
        {
            this->statusLabel.setText(tr("创建TEXT路径..."));
            //创建路径
            bool isSuccessful = itemDir.mkdir(item->data(1,0).value<FileInfo *>()->name + tr("/") + TEXT_DOC_PATH);
            if(!isSuccessful)
            {
                //创建路径失败
                return false;
            }
        }

        this->statusLabel.setText(tr("写入文件信息..."));
        std::vector<FileInfo> filesInfo;
        for(int i = 0;i < item->childCount();i++)
        {
            FileInfo *fileInfo = item->child(i)->data(1,0).value<FileInfo *>();
            FileInfo info;
            info.createTime = fileInfo->createTime;
            info.fileType = fileInfo->fileType;
            info.name = fileInfo->name;
            info.title = fileInfo->title;
            filesInfo.push_back(info);
        }

        //创建XML
        return DocInfoXML::createFilesInfo(filesInfo,item->data(1,0).value<FileInfo *>()->name + tr("/") +
                                    tr(DEFINE_ROOT_DOC_XML_NAME) + tr(DEFINE_DOC_INFO_XML_EXPANDED_NAME));
    }
}

void MainWindow::on_newDocAction_triggered()
{

}

void MainWindow::paintEvent(QPaintEvent *e)
{
    //窗体重绘时自动改变控件
    this->statusLabel.setMinimumSize(this->ui->statusBar->width() - 500,10);
    this->statusLabel.setMaximumSize(this->ui->statusBar->width() - 500,20);
}

void MainWindow::showEvent(QShowEvent *e)
{
    if(!this->isShow)
    {
        //创建状态显示设置状态显示Label
        this->statusLabel.setParent(this->ui->statusBar);
        this->statusLabel.setText(tr("就绪"));
        this->statusLabel.setMinimumSize(this->ui->statusBar->width() - 500,10);
        this->statusLabel.setMaximumSize(this->ui->statusBar->width() - 500,20);
        this->ui->statusBar->addWidget(&this->statusLabel);

        //显示文档名称
        this->nameLabel.setParent(this->ui->statusBar);
        this->nameLabel.setText(tr("文档名称：") + this->docInfo->name);
        this->nameLabel.setMinimumSize(200,10);
        this->nameLabel.setMaximumSize(200,20);
        this->ui->statusBar->addWidget(&this->nameLabel);

        //显示用户名称
        this->userNameLabel.setParent(this->ui->statusBar);
        this->userNameLabel.setText(tr("用户名称：") + this->docInfo->userName);
        this->userNameLabel.setMinimumSize(200,10);
        this->userNameLabel.setMaximumSize(200,20);
        this->ui->statusBar->addWidget(&this->userNameLabel);

        this->createRootItem();
    }
}

void MainWindow::createRootItem()
{
    this->statusLabel.setText(tr("设置根节点..."));
    //生成文件信息
    FileInfo * fileInfo = new FileInfo;
    fileInfo->createTime = this->docInfo->createTime;
    fileInfo->fileType = FileInfo::dir;
    fileInfo->name = this->systemInfo->documentPath;
    fileInfo->title = this->docInfo->name;

    this->rootItem = new QTreeWidgetItem(this->ui->treeWidget);
    this->rootItem->setText(0,fileInfo->title);
    this->rootItem->setIcon(0,QIcon(tr(":/images/tree/dir.png")));
    this->rootItem->setData(1,0,QVariant::fromValue(fileInfo));
    this->ui->treeWidget->insertTopLevelItem(0,this->rootItem);

    //检查跟节点信息XML文件是否存在
    QFile rootInfo(this->systemInfo->documentPath + tr("/") + tr(DEFINE_ROOT_DOC_XML_NAME) + tr(DEFINE_DOC_INFO_XML_EXPANDED_NAME));
    if(rootInfo.exists())
    {
        this->readChildItem(this->rootItem);
    }
    else
    {
        this->writeDocInfoXML(this->rootItem);
    }
    this->statusLabel.setText(tr("就绪"));
}
