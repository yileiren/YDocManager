#include "choseimage.h"
#include "ui_choseimage.h"

#include <QDesktopWidget>
#include <QPixmap>
#include <QDir>
#include <QMessageBox>
#include <QXmlStreamWriter>
#include <QXmlStreamReader>
#include <QFileDialog>

ChoseImage::ChoseImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoseImage),
    isShown(false)
{
    ui->setupUi(this);

    //设置窗口居中显示
    QDesktopWidget * desktop = QApplication::desktop();
    this->move((desktop->width() - this->width()) / 2,(desktop->height() - this->height()) / 2);
}

ChoseImage::~ChoseImage()
{
    delete ui;
}

void ChoseImage::on_buttonBox_accepted()
{

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
        QFile file(this->ui->fileName->text());
        QString newFileName = this->openingFile->path +
                tr("/") + HTML_DOC_PATH +
                tr("/") + IMAGES_FILE_DIR +
                tr("/");
        //file.copy( )
    }
    else
    {
        QMessageBox::information(this,tr("提示"),tr("请选择文件！"));
        return;
    }
}
