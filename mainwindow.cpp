#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "appXML/docinfoxml.h"
#include "forms/editfileinfo.h"
#include "forms/choseimage.h"
#include "forms/inserttable.h"
#include "forms/settablealgin.h"
#include "forms/initvalue.h"
#include "forms/setwidthvalue.h"
#include "forms/splitcellnum.h"
#include "forms/aboutwindow.h"

#include <QDesktopWidget>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QColorDialog>
#include <QXmlStreamReader>
#include <QFileInfo>
#include <QCloseEvent>
#include <exception>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isShow(false),
    rootItem(NULL),
    openingFile(NULL),
    isChanged(false)
{
    ui->setupUi(this);

    //添加字体选择控件
//    this->comboBox = new QFontComboBox(this->ui->toolBar);
//    this->ui->toolBar->addWidget(this->comboBox);
//    connect(this->comboBox,SIGNAL(activated(QString)),this,SLOT(changeTextFont(QString)));

    this->fontComBox = new QComboBox(this->ui->toolBar);
    this->fontComBox->setMinimumWidth(100);
    this->fontComBox->setMinimumHeight(32);
    this->ui->toolBar->addWidget(this->fontComBox);

    this->fontComBox->insertItem(0,tr(""));
    this->fontComBox->insertItem(1,tr("宋体"));
    this->fontComBox->insertItem(2,tr("黑体"));
    this->fontComBox->insertItem(3,tr("仿宋"));
    this->fontComBox->insertItem(4,tr("楷体"));
    this->fontComBox->insertItem(5,tr("隶书"));

    connect(this->fontComBox,SIGNAL(activated(QString)),this,SLOT(changeTextFont(QString)));

    //添加设置字号控件
    comboSize = new QComboBox(this->ui->toolBar);
    this->comboSize->setMinimumWidth(50);
    this->comboSize->setMinimumHeight(32);
    comboSize->setObjectName("comboSize");
    this->ui->toolBar->addWidget(comboSize);
    comboSize->setEditable(true);

    QFontDatabase db;
    foreach(int size, db.standardSizes())
        comboSize->addItem(QString::number(size));

    connect(comboSize, SIGNAL(activated(QString)),this, SLOT(changeSize(QString)));
    comboSize->setCurrentIndex(comboSize->findText(QString::number(QApplication::font().pointSize())));

    //文本样式控件
    this->comboBoxTextStyle = new QComboBox(this->ui->toolBar);
    this->comboBoxTextStyle->setMinimumWidth(100);
    this->comboBoxTextStyle->setMinimumHeight(32);
    this->comboBoxTextStyle->setIconSize(QSize(28,28));

    this->comboBoxTextStyle->addItem(tr(""));
    this->comboBoxTextStyle->addItem(QIcon(tr(":/images/textEdit/lists/1.png")),tr("项目符号1"));
    this->comboBoxTextStyle->addItem(QIcon(tr(":/images/textEdit/lists/2.png")),tr("项目符号2"));
    this->comboBoxTextStyle->addItem(QIcon(tr(":/images/textEdit/lists/3.png")),tr("项目符号3"));
    this->comboBoxTextStyle->addItem(QIcon(tr(":/images/textEdit/lists/4.png")),tr("编号1"));
    this->comboBoxTextStyle->addItem(QIcon(tr(":/images/textEdit/lists/5.png")),tr("编号2"));
    this->comboBoxTextStyle->addItem(QIcon(tr(":/images/textEdit/lists/6.png")),tr("编号3"));
    this->comboBoxTextStyle->addItem(QIcon(tr(":/images/textEdit/lists/7.png")),tr("编号4"));
    this->comboBoxTextStyle->addItem(QIcon(tr(":/images/textEdit/lists/8.png")),tr("编号5"));

    this->ui->toolBar->addWidget(this->comboBoxTextStyle);
    connect(this->comboBoxTextStyle,SIGNAL(activated(int)),this,SLOT(changeTextStyle(int)));

    //设置窗口居中显示
    QDesktopWidget * desktop = QApplication::desktop();
    this->move((desktop->width() - this->width()) / 2,(desktop->height() - this->height()) / 2);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete this->fontComBox;
    delete this->comboSize;
    delete this->comboBoxTextStyle;
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

            this->readChildItem(item);

            this->statusLabel.setText(tr("完成"));
        }
    }
}

void MainWindow::readSeccendChildItem(QTreeWidgetItem *parent)
{
    this->statusLabel.setText(tr("循环读取子节点..."));
    for(int i = 0;i < parent->childCount();i++)
    {
        if(FileInfo::dir == parent->child(i)->data(1,0).value<FileInfo *>()->fileType)
        {
            this->readChildItem(parent->child(i));
        }
    }
    this->statusLabel.setText(tr("完成"));
}

void MainWindow::deleteChildItem(QTreeWidgetItem *parent)
{
    this->statusLabel.setText(tr("循环销毁子节点..."));
    int childCount = parent->childCount();
    for(int i = 0;i < childCount;i++)
    {
        //删除子节点
        this->deleteChildItem(parent->child(0));
        //销毁文件信息
        QTreeWidgetItem *childItem = parent->child(0);
        parent->removeChild(childItem);
        delete childItem->data(1,0).value<FileInfo *>();
        delete childItem;
    }
    this->statusLabel.setText(tr("成功"));
}

void MainWindow::deleteSeccendChildItem(QTreeWidgetItem *parent)
{
    this->statusLabel.setText(tr("循环销二级节点..."));
    for(int i = 0;i < parent->childCount();i++)
    {
        this->deleteChildItem(parent->child(i));

    }
    this->statusLabel.setText(tr("成功"));
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
    //判断是否允许修改
    if(!this->docAlowOpen())
    {
        return;
    }

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
                    //创建新文档
                    if(FileInfo::doc == editFileInfo.fileInfo->fileType)
                    {
                        this->ui->yRichEditor->setReadOnly(false);
                        if(!this->writeDocFile(editFileInfo.fileInfo))
                        {
                            QMessageBox::information(this,tr("提示"),tr("创建文档失败！"),QMessageBox::Ok);
                            this->ui->treeWidget->selectedItems().at(0)->removeChild(item);
                            this->ui->yRichEditor->setReadOnly(true);
                            delete editFileInfo.fileInfo;
                            delete item;
                            this->writeDocInfoXML(this->ui->treeWidget->selectedItems().at(0));
                            this->statusLabel.setText(tr("完成"));
                            return;
                        }
                        this->ui->yRichEditor->setText(tr(""));
                        this->openingFile = editFileInfo.fileInfo;
                        this->ui->saveDocAction->setEnabled(true);
                        this->ui->editDocAction->setEnabled(false);
                        this->ui->closeDocAction->setEnabled(true);
                        this->isChanged = false;
                    }


                    this->ui->treeWidget->selectedItems().at(0)->setExpanded(true);
                    this->ui->treeWidget->selectedItems().at(0)->setSelected(false);
                    item->setSelected(true);
                }
                else
                {
                    this->ui->treeWidget->selectedItems().at(0)->removeChild(item);
                    delete item;
                    QMessageBox::information(this,tr("提示"),tr("新增失败！"),QMessageBox::Ok);
                }
                this->statusLabel.setText(tr("就绪"));
            }
            else
            {
                delete editFileInfo.fileInfo;
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
        this->statusLabel.setMinimumSize(this->width() - 500,10);
        this->statusLabel.setMaximumSize(this->width() - 500,20);
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

        //创建图像临时文件夹
        QString imagesDirName = tr(IMAGES_FILE_DIR);
        QDir imagesDir;
        if(!imagesDir.exists(imagesDirName))
        {
            imagesDir.mkdir(imagesDirName);
        }
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
    }
    else
    {
        this->writeDocInfoXML(this->rootItem);
    }
    this->statusLabel.setText(tr("就绪"));
}

void MainWindow::on_openDocAction_triggered()
{
    if(this->ui->treeWidget->selectedItems().count() > 0)
    {
        FileInfo *info = this->ui->treeWidget->selectedItems()[0]->data(1,0).value<FileInfo *>();
        if(this->docAlowOpen())
        {
            if(this->openDocFile(info))
            {
                this->ui->editDocAction->setEnabled(true);
                this->ui->saveDocAction->setEnabled(false);
                this->ui->closeDocAction->setEnabled(false);
                this->ui->openDocAction->setEnabled(false);
                this->ui->yRichEditor->setReadOnly(true);
                this->openingFile = info;
                this->isChanged = false;
            }
            else
            {
                QMessageBox::information(this,tr("提示"),tr("打开文档失败！"));
            }
        }
    }
}

void MainWindow::on_treeWidget_itemExpanded(QTreeWidgetItem* item)
{
    //this->readSeccendChildItem(this->rootItem);
}

void MainWindow::on_treeWidget_itemCollapsed(QTreeWidgetItem* item)
{
    //this->deleteSeccendChildItem(item);
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
        this->statusLabel.setText(tr("写入成功"));
        return true;
    }
    else
    {
        this->statusLabel.setText(tr("写入失败"));
        return false;
    }
}

bool MainWindow::docAlowOpen()
{
    if(this->isChanged)
    {
        int returnNum = QMessageBox::information(this,
                                                 tr("提示"),tr("当前打开的文档处于编辑状态！"),
                                                 QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,QMessageBox::Save);

        if(QMessageBox::Save == returnNum)
        {
            if(this->writeDocFile(this->openingFile))
            {
                QMessageBox::information(this,tr("提示"),tr("保存成功。"),QMessageBox::Ok);
                return true;
            }
            else
            {
                QMessageBox::information(this,tr("提示"),tr("保存失败！"),QMessageBox::Ok);
                return false;
            }
        }
        else if(QMessageBox::Discard == returnNum)
        {
            //放弃修改
            this->ui->yRichEditor->setText(tr(""));
            this->ui->yRichEditor->setReadOnly(true);
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return true;
    }
}

bool MainWindow::openDocFile(const FileInfo *fileInfo)
{
    this->statusLabel.setText(tr("清楚图像缓冲区..."));
    QDir imagesDir(tr(IMAGES_FILE_DIR));
    QFileInfoList fileInfos = imagesDir.entryInfoList(QDir::Files);
    for(int i = 0;i < fileInfos.count();i++)
    {
        QFile file(tr(IMAGES_FILE_DIR) + tr("/") + fileInfos.at(i).fileName());
        file.remove();
    }

    this->statusLabel.setText(tr("复制图像..."));
    QString infoPath = fileInfo->path +
                        tr("/") + HTML_DOC_PATH +
                        tr("/") + IMAGES_FILE_DIR +
                        tr("/") + fileInfo->name + IMAGES_INFO_FILE_EXPANDED_NAME;
    //检查文档信息
    QFile docInfoFile;
    docInfoFile.setFileName(infoPath);
    if(docInfoFile.exists())
    {
        //打开图像信息文档文件
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
                    if(name == IMAGE_FILE_INFO_TAG)
                    {
                        QString imageFileName = fileInfo->path +
                                                    tr("/") + HTML_DOC_PATH +
                                                    tr("/") + IMAGES_FILE_DIR +
                                                    tr("/") + xmlReader.attributes().value(QObject::tr(IMAGE_FILE_NAME_TAG)).toString();
                        QFile imageFile(imageFileName);
                        imageFile.copy(tr(IMAGES_FILE_DIR) + tr("/") + xmlReader.attributes().value(QObject::tr(IMAGE_FILE_NAME_TAG)).toString());

                    }

                    xmlReader.readNext();
                }
                else
                {
                    xmlReader.readNext();
                }
            }
            docInfoFile.close();
        }
        else
        {
            return false;
        }
    }

    this->statusLabel.setText(tr("打开文档..."));

    //读取HTML文件
    QFile newDocFile(fileInfo->path + HTML_DOC_PATH + tr("/") + fileInfo->name + tr(".html"));
    if(!newDocFile.open(QFile::ReadOnly))
    {
        return false;
    }
    QTextStream tsHTML(&newDocFile);
    tsHTML.setCodec("utf-8");
    this->ui->yRichEditor->setHtml(tsHTML.readAll());
    newDocFile.close();

    this->statusLabel.setText(tr("完成"));
    return true;
}

void MainWindow::on_treeWidget_itemSelectionChanged()
{
    //获取当前选中的文件信息
    if(this->ui->treeWidget->selectedItems().count() > 0)
    {
        FileInfo *fileInfo = this->ui->treeWidget->selectedItems()[0]->data(1,0).value<FileInfo *>();
        if(FileInfo::dir == fileInfo->fileType)
        {
            this->ui->newDocAction->setEnabled(true);//选中的是目录时启用新建文档Action
            this->ui->openDocAction->setEnabled(false);//禁用打开文档
        }

        if(FileInfo::doc == fileInfo->fileType)
        {
            if(this->openingFile != NULL && this->openingFile->name == fileInfo->name)
            {
                this->ui->openDocAction->setEnabled(false);//当选中的文档已经打开禁用打开
            }
            else
            {
                this->ui->openDocAction->setEnabled(true);
            }

            this->ui->newDocAction->setEnabled(false);//选中的是目录时启用新建文档Action
        }
    }
}

void MainWindow::on_saveDocAction_triggered()
{
    if(this->writeDocFile(this->openingFile))
    {
        this->isChanged = false;
        QMessageBox::information(this,tr("提示"),tr("保存成功！"),QMessageBox::Ok);
    }
    else
    {
        QMessageBox::information(this,tr("提示"),tr("保存失败！"),QMessageBox::Ok);
    }
}

void MainWindow::on_editDocAction_triggered()
{
    this->ui->yRichEditor->setReadOnly(false);
    this->ui->saveDocAction->setEnabled(true);
    this->ui->editDocAction->setEnabled(false);
    this->ui->closeDocAction->setEnabled(true);
    this->isChanged = false;
    this->changeMenuState(this->ui->yRichEditor->currentCharFormat().font());
}

void MainWindow::on_closeDocAction_triggered()
{
    int returnNum = QMessageBox::Discard;
    if(this->isChanged)
    {
        returnNum = QMessageBox::information(this,
                                             tr("提示"),tr("当前打开的文档处于编辑状态！"),
                                             QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,QMessageBox::Save);
    }

    if(QMessageBox::Save == returnNum)
    {
        if(this->writeDocFile(this->openingFile))
        {
            QMessageBox::information(this,tr("提示"),tr("保存成功。"),QMessageBox::Ok);
            this->ui->yRichEditor->setReadOnly(true);
            this->isChanged = false;
        }
        else
        {
            QMessageBox::information(this,tr("提示"),tr("保存失败！"),QMessageBox::Ok);
            return;
        }
    }
    else if(QMessageBox::Discard == returnNum)
    {
        //放弃修改
        this->ui->yRichEditor->setReadOnly(true);
        this->openDocFile(this->openingFile);
        this->isChanged = false;
    }
    else
    {
        return;
    }

    this->ui->saveDocAction->setEnabled(false);
    this->ui->editDocAction->setEnabled(true);
    this->ui->closeDocAction->setEnabled(false);
}

void MainWindow::on_setBoldAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        this->ui->yRichEditor->wordBold(this->ui->setBoldAction->isChecked());
    }
}

void MainWindow::on_undoAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        this->ui->yRichEditor->undo();
    }
}

void MainWindow::on_redoAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        this->ui->yRichEditor->redo();
    }
}

void MainWindow::on_setItalicAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        this->ui->yRichEditor->wordItalic(this->ui->setItalicAction->isChecked());
    }
}

void MainWindow::on_setUnderlineAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        this->ui->yRichEditor->wordUnderline(this->ui->setUnderlineAction->isChecked());
    }
}

void MainWindow::on_yRichEditor_currentCharFormatChanged(QTextCharFormat format)
{
    this->changeMenuState(format.font());
}

void MainWindow::changeMenuState(const QFont &f)
{
    this->ui->setBoldAction->setChecked(f.bold());
    this->ui->setItalicAction->setChecked(f.italic());
    this->ui->setUnderlineAction->setChecked(f.underline());
    this->fontComBox->setCurrentIndex(this->fontComBox->findText(QFontInfo(f).family()));
    this->comboSize->setCurrentIndex(this->comboSize->findText(QString::number(f.pointSize())));

    this->changeTextAligenAction();

//    //改变文本列表

//    if(QTextListFormat::ListStyleUndefined == this->ui->yRichEditor->getTextStyle())
//    {
//        this->comboBoxTextStyle->setCurrentIndex(0);
//    }
//    else if(QTextListFormat::ListDisc == this->ui->yRichEditor->getTextStyle())
//    {
//        this->comboBoxTextStyle->setCurrentIndex(1);
//    }
//    else if(QTextListFormat::ListCircle == this->ui->yRichEditor->getTextStyle())
//    {
//        this->comboBoxTextStyle->setCurrentIndex(2);
//    }
//    else if(QTextListFormat::ListSquare == this->ui->yRichEditor->getTextStyle())
//    {
//        this->comboBoxTextStyle->setCurrentIndex(3);
//    }
//    else if(QTextListFormat::ListDecimal == this->ui->yRichEditor->getTextStyle())
//    {
//        this->comboBoxTextStyle->setCurrentIndex(4);
//    }
//    else if(QTextListFormat::ListLowerAlpha == this->ui->yRichEditor->getTextStyle())
//    {
//        this->comboBoxTextStyle->setCurrentIndex(5);
//    }
//    else if(QTextListFormat::ListUpperAlpha == this->ui->yRichEditor->getTextStyle())
//    {
//        this->comboBoxTextStyle->setCurrentIndex(6);
//    }
//    else if(QTextListFormat::ListLowerRoman == this->ui->yRichEditor->getTextStyle())
//    {
//        this->comboBoxTextStyle->setCurrentIndex(7);
//    }
//    else if(QTextListFormat::ListUpperRoman == this->ui->yRichEditor->getTextStyle())
//    {
//        this->comboBoxTextStyle->setCurrentIndex(8);
//    }
}

void MainWindow::changeTextAligenAction()
{
    //设置对齐方式按钮
    if(this->ui->yRichEditor->getTextAlign() == YRichEditor::Left)
    {
        this->ui->setTextLeftAction->setChecked(true);
        this->ui->setTextCenterAction->setChecked(false);
        this->ui->setTextRightAction->setChecked(false);
    }
    else if(this->ui->yRichEditor->getTextAlign() == YRichEditor::Center)
    {
        this->ui->setTextLeftAction->setChecked(false);
        this->ui->setTextCenterAction->setChecked(true);
        this->ui->setTextRightAction->setChecked(false);
    }
    else if(this->ui->yRichEditor->getTextAlign() == YRichEditor::Right)
    {
        this->ui->setTextLeftAction->setChecked(false);
        this->ui->setTextCenterAction->setChecked(false);
        this->ui->setTextRightAction->setChecked(true);
    }
    else
    {
        this->ui->setTextLeftAction->setChecked(true);
        this->ui->setTextCenterAction->setChecked(false);
        this->ui->setTextRightAction->setChecked(false);
    }
}

void MainWindow::on_yRichEditor_textChanged()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        this->isChanged = true;
    }
}

void MainWindow::changeTextFont(QString f)
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        this->ui->yRichEditor->wordFamily(f);
    }
}

void MainWindow::changeSize(QString s)
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        this->ui->yRichEditor->wordSize(s.toInt());
    }
}

void MainWindow::on_setTextLeftAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        this->ui->yRichEditor->textAlign(YRichEditor::Left);
        this->changeTextAligenAction();
    }
}

void MainWindow::on_setTextCenterAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        if(this->ui->setTextCenterAction->isChecked())
        {
            this->ui->yRichEditor->textAlign(YRichEditor::Center);
        }
        else
        {
            this->ui->yRichEditor->textAlign(YRichEditor::Left);
        }
        this->changeTextAligenAction();
    }
}

void MainWindow::on_setTextRightAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        if(this->ui->setTextRightAction->isChecked())
        {
            this->ui->yRichEditor->textAlign(YRichEditor::Right);
        }
        else
        {
            this->ui->yRichEditor->textAlign(YRichEditor::Right);
        }
        this->changeTextAligenAction();
    }
}

void MainWindow::on_fontColorAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        QColorDialog colorDlg(this->ui->yRichEditor->getWordColor(),this);
        if(QDialog::Accepted ==  colorDlg.exec())
        {
            this->ui->yRichEditor->wordColor(colorDlg.selectedColor());
        }
    }
}

void MainWindow::on_backgroundColorAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        QColorDialog colorDlg(this->ui->yRichEditor->getWordBackgroundColor(),this);
        if(QDialog::Accepted ==  colorDlg.exec())
        {
            this->ui->yRichEditor->wordBackgroundColor(colorDlg.selectedColor());
        }
    }
}

void MainWindow::changeTextStyle(int index)
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        if(index == 0)
        {
            this->ui->yRichEditor->textStyle(QTextListFormat::ListStyleUndefined);
        }
        else if(index == 1)
        {
            this->ui->yRichEditor->textStyle(QTextListFormat::ListDisc);
        }
        else if(index == 2)
        {
            this->ui->yRichEditor->textStyle(QTextListFormat::ListCircle);
        }
        else if(index == 3)
        {
            this->ui->yRichEditor->textStyle(QTextListFormat::ListSquare);
        }
        else if(index == 4)
        {
            this->ui->yRichEditor->textStyle(QTextListFormat::ListDecimal);
        }
        else if(index == 5)
        {
            this->ui->yRichEditor->textStyle(QTextListFormat::ListLowerAlpha);
        }
        else if(index == 6)
        {
            this->ui->yRichEditor->textStyle(QTextListFormat::ListUpperAlpha);
        }
        else if(index == 7)
        {
            this->ui->yRichEditor->textStyle(QTextListFormat::ListLowerRoman);
        }
        else if(index == 8)
        {
            this->ui->yRichEditor->textStyle(QTextListFormat::ListUpperRoman);
        }
    }
}

void MainWindow::on_insertTableAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        InsertTable dlg;
        if(QDialog::Accepted == dlg.exec())
        {
            this->ui->yRichEditor->insertTable(dlg.getRowCount(),
                                               dlg.getColoumCount(),
                                               dlg.getCellsSpacing(),
                                               dlg.getCellsSpacing(),
                                               dlg.getTableAlign(),
                                               dlg.getBackgroundColor());
        }
    }
}

void MainWindow::on_insertImageAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        ChoseImage choseImage;
        choseImage.openingFile = this->openingFile;
        if(QDialog::Accepted == choseImage.exec())
        {
            this->ui->yRichEditor->insertImage(tr(IMAGES_FILE_DIR) + tr("/") + choseImage.selectedText);
        }
    }
}

void MainWindow::on_setTableAlignAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        SetTableAlgin dlg;
        dlg.setTableAlgin(this->ui->yRichEditor->getTableAlign());
        if(QDialog::Accepted == dlg.exec())
        {
            this->ui->yRichEditor->setTableAlign(dlg.getTableAlign());
        }
    }
}

void MainWindow::on_setTableBackGroundColorAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        QColorDialog colorDlg(this->ui->yRichEditor->getTableBackGroundColor(),this);
        if(QDialog::Accepted ==  colorDlg.exec())
        {
            this->ui->yRichEditor->setTableBackGroundColor(colorDlg.selectedColor());
        }
    }
}

void MainWindow::on_insertRowsAtTopAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        InitValue dlg;
        if(QDialog::Accepted ==  dlg.exec())
        {
            this->ui->yRichEditor->insertRows(this->ui->yRichEditor->getPositionCell().row,
                                              dlg.getInputValue(),
                                              false);
        }
    }
}

void MainWindow::on_insertRowsAtBottomAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        InitValue dlg;
        if(QDialog::Accepted ==  dlg.exec())
        {
            this->ui->yRichEditor->insertRows(this->ui->yRichEditor->getPositionCell().row,
                                              dlg.getInputValue(),
                                              true);
        }
    }
}

void MainWindow::on_insertColumnsAtBeginAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        InitValue dlg;
        if(QDialog::Accepted ==  dlg.exec())
        {
            this->ui->yRichEditor->insertColumns(this->ui->yRichEditor->getPositionCell().column,
                                                 dlg.getInputValue(),
                                                 false);
        }
    }
}

void MainWindow::on_insertColumnsAtEndAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        InitValue dlg;
        if(QDialog::Accepted ==  dlg.exec())
        {
            this->ui->yRichEditor->insertColumns(this->ui->yRichEditor->getPositionCell().column,
                                                 dlg.getInputValue(),
                                                 true);
        }
    }
}

void MainWindow::on_removeRowsAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        this->ui->yRichEditor->removeRows();
    }
}

void MainWindow::on_removeColumnsAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        this->ui->yRichEditor->removeColumns();
    }
}

void MainWindow::on_setColumnsWidthAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        QVector<QTextLength> l = this->ui->yRichEditor->getColumnsWidth();

        if(l.count() > 0)
        {
            int p = this->ui->yRichEditor->getPositionCell().column;

            if(p >= 0)
            {
                int w = l[p].rawValue();
                QTextLength::Type t = this->ui->yRichEditor->getColumnsWidth()[this->ui->yRichEditor->getPositionCell().column].type();

                SetWidthValue dlg;
                dlg.setValue(w);
                dlg.setValueType(t);
                if(QDialog::Accepted == dlg.exec())
                {
                    this->ui->yRichEditor->setColumnsWidth(dlg.getValue(),dlg.getValueType());
                }
            }
        }
    }
}

void MainWindow::resizeEvent(QResizeEvent * e)
{
    this->statusLabel.setMinimumSize(this->width() - 500,10);
    this->statusLabel.setMaximumSize(this->width() - 500,20);
}

void MainWindow::on_mergeCellsAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        this->ui->yRichEditor->mergeCells();
    }
}

void MainWindow::on_splitCellAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        int r = this->ui->yRichEditor->getPositionCell().row;
        int c = this->ui->yRichEditor->getPositionCell().column;

        if(r >= 0 && c >= 0)
        {
            SplitCellNum dlg;
            if(QDialog::Accepted == dlg.exec())
            {
                this->ui->yRichEditor->splitCell(r,c,dlg.getRowCount() - 1,dlg.getColumnCount() - 1);
            }
        }
    }
}

void MainWindow::on_setTableCellsPaddingAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        InitValue dlg;
        dlg.setInputValue(this->ui->yRichEditor->getTableCellsPadding());
        if(QDialog::Accepted ==  dlg.exec())
        {
            this->ui->yRichEditor->setTableCellsPadding(dlg.getInputValue());
        }
    }
}

void MainWindow::on_setTableCellsSpacingAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        InitValue dlg;
        dlg.setInputValue(this->ui->yRichEditor->getTableCellsSpacing());
        if(QDialog::Accepted ==  dlg.exec())
        {
            this->ui->yRichEditor->setTableCellsSpacing(dlg.getInputValue());
        }
    }
}

void MainWindow::on_setTableCellsBackGroundColorAction_triggered()
{
    if(!this->ui->yRichEditor->isReadOnly())
    {
        QColorDialog colorDlg(this->ui->yRichEditor->getTableCellBackGroundColor(),this);
        if(QDialog::Accepted ==  colorDlg.exec())
        {
            this->ui->yRichEditor->setTableCellsBackGroundColor(colorDlg.selectedColor());
        }
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(this->docAlowOpen())
    {
        e->accept();
    }
    else
    {
        e->ignore();
    }
}

void MainWindow::on_exitAction_triggered()
{
    this->close();
}

void MainWindow::showAboutWindow()
{
    AboutWindow dlg;
    dlg.systemInfo = this->systemInfo;
    dlg.docInfo = this->docInfo;
    dlg.exec();
}

void MainWindow::on_action_triggered()
{
    this->showAboutWindow();
}
