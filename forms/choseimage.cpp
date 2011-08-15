#include "choseimage.h"
#include "ui_choseimage.h"

#include <QDesktopWidget>
#include <QPixmap>
#include <QDir>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFileDialog>
#include <QUuid>

ChoseImage::ChoseImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoseImage),
    isShown(false),
    selectedText(tr(""))
{
    ui->setupUi(this);

    //设置窗口居中显示
    QDesktopWidget * desktop = QApplication::desktop();
    this->move((desktop->width() - this->width()) / 2,(desktop->height() - this->height()) / 2);

    //禁用确定按钮
    this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
}

ChoseImage::~ChoseImage()
{
    delete ui;
}

void ChoseImage::on_buttonBox_accepted()
{
    if(this->ui->listWidget->selectedItems().count() > 0)
    {
        this->selectedText = this->ui->listWidget->selectedItems()[0]->text();
    }
}

void ChoseImage::showEvent(QShowEvent *e)
{
    if(!this->isShown)
    {
        //判断图片文件夹是否存在
        QString imagesPath = this->openingFile->path +
                tr("/") + HTML_DOC_PATH +
                tr("/") + IMAGES_FILE_DIR;
        QDir dir;

        if(!dir.exists(imagesPath))
        {
            if(!dir.mkdir(imagesPath))
            {
                QMessageBox::information(this,tr("提示"),tr("创建图片文件夹失败！"),QMessageBox::Ok);
                return;
            }
        }

        //判断图像信息文件是否存在
        QFile file;
        if(file.exists(imagesPath + tr("/") + tr("/") + this->openingFile->name + IMAGES_INFO_FILE_EXPANDED_NAME))
        {
            this->readImages();
        }
        else
        {
            this->writeImages();
        }
        this->isShown = true;
    }
}

void ChoseImage::writeImages()
{
    QString infoPath = this->openingFile->path +
                        tr("/") + HTML_DOC_PATH +
                        tr("/") + IMAGES_FILE_DIR +
                        tr("/") + this->openingFile->name + IMAGES_INFO_FILE_EXPANDED_NAME;

    QFile file(infoPath);
    if(!file.open(QFile::WriteOnly|QFile::Text))
    {
        QMessageBox::information(this,tr("信息"),tr("写入图像信息文件失败！"));
        return;
    }

    QXmlStreamWriter *xmlWriter = new QXmlStreamWriter(&file);
    xmlWriter->setAutoFormatting(true);
    xmlWriter->writeStartDocument();

    //写入跟节点
    xmlWriter->writeStartElement(tr(IMAGE_FILES_INFO_ROOT_TAG));

    //this->ui->listWidget->insertItem();
    //循环写入文件信息
    for(int i = 0;i < this->ui->listWidget->count();i++)
    {
        xmlWriter->writeStartElement(IMAGE_FILE_INFO_TAG);
        xmlWriter->writeAttribute(QObject::tr(IMAGE_FILE_NAME_TAG),this->ui->listWidget->item(i)->text());
        xmlWriter->writeEndElement();

    }

    xmlWriter->writeEndElement();

    xmlWriter->writeEndDocument();
    delete xmlWriter;
    file.close();
}

void ChoseImage::readImages()
{
    QString infoPath = this->openingFile->path +
                        tr("/") + HTML_DOC_PATH +
                        tr("/") + IMAGES_FILE_DIR +
                        tr("/") + this->openingFile->name + IMAGES_INFO_FILE_EXPANDED_NAME;
    //检查文档信息
    QFile docInfoFile;
    docInfoFile.setFileName(infoPath);

    //打开文件
    if(docInfoFile.open(QFile::ReadOnly))
    {
        //读取文档信息
        QXmlStreamReader xmlReader;
        xmlReader.setDevice(&docInfoFile);
        while(!xmlReader.atEnd())
        {
            FileInfo fileInfo;

            if(xmlReader.isStartElement())
            {
                QString name = xmlReader.name().toString();
                if(name == IMAGE_FILE_INFO_TAG)
                {
                    this->ui->listWidget->insertItem(
                                this->ui->listWidget->count(),
                                xmlReader.attributes().value(QObject::tr(IMAGE_FILE_NAME_TAG)).toString());
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
        QMessageBox::information(this,tr("信息"),tr("读取图像信息文件失败！"));
        return;
    }
}

void ChoseImage::on_pushButton_clicked()
{
    //获取模型路径
        QString filePath = QFileDialog::getOpenFileName(this,tr("打开图片"), "", tr(IMAGE_FILES_TYPE));

        if(!filePath.isNull())
        {
            this->ui->fileName->setText(filePath);
        }
}

void ChoseImage::on_pushButton_2_clicked()
{
    if(this->ui->fileName->text().length() > 0)
    {
        //获取扩展名
        QString selectedFileName = this->ui->fileName->text();
        QStringList list1 = selectedFileName.split(tr("."));
        QString fileName = QUuid::createUuid() + tr(".") + list1[list1.count() -1];

        //拷贝图片到系统目录
        QFile file(this->ui->fileName->text());
        QString newFileName = this->openingFile->path +
                tr("/") + HTML_DOC_PATH +
                tr("/") + IMAGES_FILE_DIR +
                tr("/") + fileName;
        file.copy(newFileName);

        //添加图像到列表
        this->ui->listWidget->addItem(fileName);

        //保存
        this->writeImages();
    }
    else
    {
        QMessageBox::information(this,tr("提示"),tr("请选择文件！"));
        return;
    }
}

void ChoseImage::on_listWidget_itemSelectionChanged()
{
    if(this->ui->listWidget->selectedItems().count() > 0)
    {
        //显示图像
        QString fileName = this->openingFile->path +
                tr("/") + HTML_DOC_PATH +
                tr("/") + IMAGES_FILE_DIR +
                tr("/") + this->ui->listWidget->selectedItems()[0]->text();
        QPixmap pimmap(fileName);

        //修改图像大小
        if(pimmap.width() > this->ui->label->width() || pimmap.height() > this->ui->label->height())
        {
            this->ui->label->setPixmap(pimmap.scaled(this->ui->label->width(),this->ui->label->height(),Qt::KeepAspectRatio));
        }
        else
        {
            this->ui->label->setPixmap(pimmap);
        }

        this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
}

void ChoseImage::on_pushButton_3_clicked()
{
    if(this->ui->listWidget->selectedItems().count() > 0)
    {
        if(QMessageBox::Yes == QMessageBox::information(this,tr("提示"),tr("确定要删除选中的图片？"),
                                                        QMessageBox::Yes | QMessageBox::No,QMessageBox::No))
        {
            QString fileName = this->openingFile->path +
                    tr("/") + HTML_DOC_PATH +
                    tr("/") + IMAGES_FILE_DIR +
                    tr("/") + this->ui->listWidget->selectedItems()[0]->text();

            //删除图像
            if(QFile::remove(fileName))
            {
                this->ui->listWidget->removeItemWidget(this->ui->listWidget->selectedItems()[0]);
                delete this->ui->listWidget->selectedItems()[0];

                this->writeImages();
            }
            else
            {
                QMessageBox::information(this,tr("提示"),tr("删除图像失败！"),QMessageBox::Ok);
            }
        }


    }
    else
    {
        QMessageBox::information(this,tr("提示"),tr("请选择要移除的图片！"),QMessageBox::Ok);
    }
}
