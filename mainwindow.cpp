#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "appXML/docinfoxml.h"
#include "forms/editfileinfo.h"

#include <QDesktopWidget>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QTextStream>

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
        std::vector<FileInfo> filesInfo = DocInfoXML::readFilesInfo(fileInfo->path + fileInfo->name + tr("/") +
                                                                    tr(DEFINE_ROOT_DOC_XML_NAME) + tr(DEFINE_DOC_INFO_XML_EXPANDED_NAME));

        //循环设置节点
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
            FileInfo *info = new FileInfo;
            info->createTime = filesInfo[s].createTime;
            info->fileType = filesInfo[s].fileType;
            info->name = filesInfo[s].name;
            info->path = filesInfo[s].path;
            info->title = filesInfo[s].title;
            item->setData(1,0,QVariant::fromValue(info));

        }
    }
}

void MainWindow::readSeccendChildItem(QTreeWidgetItem *parent)
{
    this->statusLabel.setText(tr("循环读取子节点..."));
    for(int i = 0;i < parent->childCount();i++)
    {
        if(FileInfo::dir == parent->data(1,0).value<FileInfo *>()->fileType)
        {
            this->readChildItem(parent->child(i));
        }
    }
}

bool MainWindow::writeDocInfoXML(const QTreeWidgetItem *item)
{
    //获取文件信息
    if(FileInfo::dir == item->data(1,0).value<FileInfo *>()->fileType)
    {
        this->statusLabel.setText(tr("写入文件信息..."));
        //判断文件夹是否存在
        QDir itemDir;
        bool isDirExitsts = itemDir.exists(item->data(1,0).value<FileInfo *>()->path +
                                           item->data(1,0).value<FileInfo *>()->name);
        if(!isDirExitsts)
        {
            this->statusLabel.setText(tr("创建文档路径..."));
            //创建路径
            bool isSuccessful = itemDir.mkdir(item->data(1,0).value<FileInfo *>()->path +
                                              item->data(1,0).value<FileInfo *>()->name);
            if(!isSuccessful)
            {
                //创建路径失败
                return false;
            }
        }

        //检查HTML文档存储文件夹
        isDirExitsts = itemDir.exists(item->data(1,0).value<FileInfo *>()->path +
                                      item->data(1,0).value<FileInfo *>()->name + tr("/") + HTML_DOC_PATH);
        if(!isDirExitsts)
        {
            this->statusLabel.setText(tr("创建HTML路径..."));
            //创建路径
            bool isSuccessful = itemDir.mkdir(item->data(1,0).value<FileInfo *>()->path +
                                              item->data(1,0).value<FileInfo *>()->name + tr("/") + HTML_DOC_PATH);
            if(!isSuccessful)
            {
                //创建路径失败
                return false;
            }
        }

        //检查TEXT文档存储文件夹
        isDirExitsts = itemDir.exists(item->data(1,0).value<FileInfo *>()->path +
                                      item->data(1,0).value<FileInfo *>()->name + tr("/") + TEXT_DOC_PATH);
        if(!isDirExitsts)
        {
            this->statusLabel.setText(tr("创建TEXT路径..."));
            //创建路径
            bool isSuccessful = itemDir.mkdir(item->data(1,0).value<FileInfo *>()->path +
                                              item->data(1,0).value<FileInfo *>()->name + tr("/") + TEXT_DOC_PATH);
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
            info.path = fileInfo->path;
            info.title = fileInfo->title;
            filesInfo.push_back(info);
        }

        //创建XML
        return DocInfoXML::createFilesInfo(filesInfo,item->data(1,0).value<FileInfo *>()->path +
                                           item->data(1,0).value<FileInfo *>()->name + tr("/") +
                                    tr(DEFINE_ROOT_DOC_XML_NAME) + tr(DEFINE_DOC_INFO_XML_EXPANDED_NAME));
    }
    else
    {
        return false;
    }
}

void MainWindow::on_newDocAction_triggered()
{
    //判断选中的节点
    if(this->ui->treeWidget->selectedItems().count() > 0)
    {
        if(FileInfo::dir == this->ui->treeWidget->selectedItems().at(0)->data(1,0).value<FileInfo *>()->fileType)
        {
            EditFileInfo editFileInfo;
            editFileInfo.parentFileInfo = this->ui->treeWidget->selectedItems().at(0)->data(1,0).value<FileInfo *>();
            if(QDialog::Accepted == editFileInfo.exec())
            {
                //往树中插入节点
                this->statusLabel.setText(tr("插入节点..."));
                QTreeWidgetItem * item = new QTreeWidgetItem(this->ui->treeWidget->selectedItems().at(0));
                item->setText(0,editFileInfo.fileInfo->title);

                if(FileInfo::dir == editFileInfo.fileInfo->fileType)
                {
                    item->setIcon(0,QIcon(tr(":/images/tree/dir.png")));
                }
                else if(FileInfo::doc == editFileInfo.fileInfo->fileType)
                {
                    item->setIcon(0,QIcon(tr(":/images/tree/doc.png")));
                }

                item->setData(1,0,QVariant::fromValue(editFileInfo.fileInfo));

                //将节点写入XML
                this->statusLabel.setText(tr("将节点写入XML..."));
                if(this->writeDocInfoXML(this->ui->treeWidget->selectedItems().at(0)))
                {
                    this->ui->treeWidget->selectedItems().at(0)->setExpanded(true);
                    this->ui->treeWidget->selectedItems().at(0)->setSelected(false);
                    item->setSelected(true);

                    //创建新文档
                    if(FileInfo::doc == editFileInfo.fileInfo->fileType)
                    {
                        this->ui->yRichEditor->setReadOnly(false);
                        if(!this->writeDocFile(editFileInfo.fileInfo))
                        {
                            QMessageBox::information(this,tr("提示"),tr("创建文档失败！"),QMessageBox::Ok);
                        }
                    }
                }
                else
                {
                    this->ui->treeWidget->selectedItems().at(0)->removeChild(item);
                    delete item;
                    QMessageBox::information(this,tr("提示"),tr("新增失败！"),QMessageBox::Ok);
                }
                this->statusLabel.setText(tr("就绪"));
            }
        }
        else
        {
            QMessageBox::information(this,tr("提示！"),tr("请选中一个目录节点！"),QMessageBox::Ok);
            return;
        }
    }
    else
    {
        QMessageBox::information(this,tr("提示！"),tr("请选中一个目录节点！"),QMessageBox::Ok);
        return;
    }
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
    fileInfo->path = tr("");
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
        this->statusLabel.setText(tr("读取二级节点..."));
    }
    else
    {
        this->writeDocInfoXML(this->rootItem);
    }
    this->statusLabel.setText(tr("就绪"));
}

void MainWindow::on_openDocAction_triggered()
{
    this->statusLabel.setText(tr("dd"));
}

void MainWindow::on_treeWidget_itemExpanded(QTreeWidgetItem* item)
{
    QMessageBox::information(this,tr("提示！"),tr("ee"),QMessageBox::Ok);
}

void MainWindow::on_treeWidget_itemCollapsed(QTreeWidgetItem* item)
{
    QMessageBox::information(this,tr("提示！"),tr("d"),QMessageBox::Ok);
}

bool MainWindow::writeDocFile(const FileInfo *fileInfo)
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        this->statusLabel.setText(tr("写入文档..."));

        //创建HTML文件
        QFile newDocFile(fileInfo->path + HTML_DOC_PATH + tr("/") + fileInfo->name + tr(".html"));
        if(!newDocFile.open(QFile::WriteOnly|QFile::Truncate))
        {
            return false;
        }
        QTextStream tsHTML(&newDocFile);
        tsHTML.setCodec("utf-8");

        //写入默认系统信息
        tsHTML << this->ui->yRichEditor->toHtml();
        newDocFile.close();

        //创建TEXT文件
        newDocFile.setFileName(fileInfo->path + TEXT_DOC_PATH + tr("/") + fileInfo->name + tr(".txt"));
        if(!newDocFile.open(QFile::WriteOnly|QFile::Truncate))
        {
            return false;
        }
        QTextStream tsTEXT(&newDocFile);
        tsTEXT.setCodec("utf-8");

        //写入默认系统信息
        tsTEXT << this->ui->yRichEditor->toPlainText();
        newDocFile.close();
        this->statusLabel.setText(tr("写入成功..."));
        return true;
    }
    else
    {
        return false;
    }
}
