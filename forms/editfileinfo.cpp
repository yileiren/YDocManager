#include "editfileinfo.h"
#include "ui_editfileinfo.h"

#include <QDesktopWidget>
#include <QDialogButtonBox>
#include <qpushbutton>
#include <QDateTime>
#include <QUuid>

EditFileInfo::EditFileInfo(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditFileInfo),
    isShown(false),
    fileInfo(NULL)
{
    ui->setupUi(this);

    //设置窗口居中显示
    QDesktopWidget * desktop = QApplication::desktop();
    this->move((desktop->width() - this->width()) / 2,(desktop->height() - this->height()) / 2);
}

EditFileInfo::~EditFileInfo()
{
    delete ui;
}

void EditFileInfo::showEvent(QShowEvent *e)
{
    if(!this->isShown)
    {
        //设置文件信息
        if(this->fileInfo == NULL)
        {
            this->fileInfo = new FileInfo;
            this->fileInfo->createTime = QDateTime::currentDateTime();
            this->fileInfo->fileType = FileInfo::doc;
            this->fileInfo->name = QUuid::createUuid().toString();
            if(this->parentFileInfo->path != tr(""))
            {
                this->fileInfo->path = this->parentFileInfo->path + this->parentFileInfo->name + tr("/");
            }
            else
            {
                this->fileInfo->path = this->parentFileInfo->name + tr("/");
            }
            this->fileInfo->title = tr("");
        }
        this->ui->lineEditTitle->setText(this->fileInfo->title);
        this->ui->labelCreateTime->setText(this->fileInfo->createTime.toString(tr(DATE_TIME_FORMAT)));
        if(FileInfo::dir == this->fileInfo->fileType)
        {
            this->ui->radioButtonDir->setChecked(true);
        }

        //禁用确定
        if(this->ui->lineEditTitle->text() == tr(""))
        {
            this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        }
    }
}

void EditFileInfo::on_radioButtonFile_clicked(bool checked)
{
    if(checked)
    {
        this->fileInfo->fileType = FileInfo::doc;
    }
}

void EditFileInfo::on_radioButtonDir_clicked(bool checked)
{
    if(checked)
    {
        this->fileInfo->fileType = FileInfo::dir;
    }
}

void EditFileInfo::on_lineEditTitle_textChanged(QString str)
{
    if(str.length() > 0)
    {
        this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    }
    else
    {
        this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}

void EditFileInfo::on_lineEditTitle_editingFinished()
{
    this->fileInfo->title = this->ui->lineEditTitle->text();
}
