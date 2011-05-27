#include "docinfoedit.h"
#include "ui_docinfoedit.h"

#include <QUuid>
#include <QMessageBox>
#include <QCloseEvent>
#include <QPushButton>
#include <QDesktopWidget>

DocInfoEdit::DocInfoEdit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DocInfoEdit),
    isShown(false)
{
    ui->setupUi(this);
    this->ui->dateTimeEditCreateTime->setDateTime(QDateTime::currentDateTime());

    //设置窗口居中显示
    QDesktopWidget * desktop = QApplication::desktop();
    this->move((desktop->width() - this->width()) / 2,(desktop->height() - this->height()) / 2);
}

DocInfoEdit::~DocInfoEdit()
{
    delete ui;
}

void DocInfoEdit::showEvent(QShowEvent *e)
{
    //第一次显示时
    if(!this->isShown)
    {
        //创建uuid
        if(this->docInfo->uuid == tr(""))
        {
            this->docInfo->uuid == QUuid::createUuid().toString();
        }

        //设置文档根XML名称
        if(this->docInfo->rootXML == tr(""))
        {
            this->docInfo->rootXML == tr(DEFINE_ROOT_DOC_XML_NAME) + tr(EXPANDED_NAME);
        }

        //设置文档信息
        this->ui->linEditName->setText(this->docInfo->name);
        this->ui->lineEditDocNum->setText(this->docInfo->num);
        this->ui->lineEditUserName->setText(this->docInfo->userName);
        this->ui->lineEditUserNum->setText(this->docInfo->userNum);

        //设置确定按钮状态
        this->changeOKButton();
    }
}


void DocInfoEdit::on_linEditName_editingFinished()
{
    if(this->ui->linEditName->text().length() <= 0)
    {
        this->ui->linEditName->setStyleSheet(tr("background-color: rgb(255, 0, 0);"));
    }
    this->changeOKButton();
}


void DocInfoEdit::on_linEditName_textChanged(QString str)
{
    if(str.length() <= 0)
    {
        this->ui->linEditName->setStyleSheet(tr("background-color: rgb(255, 0, 0);"));
    }
    else
    {
        this->ui->linEditName->setStyleSheet(tr(""));
    }
    this->changeOKButton();
}

void DocInfoEdit::on_lineEditDocNum_editingFinished()
{
    if(this->ui->lineEditDocNum->text().length() <= 0)
    {
        this->ui->lineEditDocNum->setStyleSheet(tr("background-color: rgb(255, 0, 0);"));
    }
    this->changeOKButton();
}

void DocInfoEdit::on_lineEditDocNum_textChanged(QString str)
{
    if(str.length() <= 0)
    {
        this->ui->lineEditDocNum->setStyleSheet(tr("background-color: rgb(255, 0, 0);"));
    }
    else
    {
        this->ui->lineEditDocNum->setStyleSheet(tr(""));
    }
    this->changeOKButton();
}

void DocInfoEdit::on_lineEditUserName_editingFinished()
{
    if(this->ui->lineEditUserName->text().length() <= 0)
    {
        this->ui->lineEditUserName->setStyleSheet(tr("background-color: rgb(255, 0, 0);"));
    }
    this->changeOKButton();
}

void DocInfoEdit::on_lineEditUserName_textChanged(QString str)
{
    if(str.length() <= 0)
    {
        this->ui->lineEditUserName->setStyleSheet(tr("background-color: rgb(255, 0, 0);"));
    }
    else
    {
        this->ui->lineEditUserName->setStyleSheet(tr(""));
    }
    this->changeOKButton();
}

void DocInfoEdit::on_lineEditUserNum_editingFinished()
{
    if(this->ui->lineEditUserNum->text().length() <= 0)
    {
        this->ui->lineEditUserNum->setStyleSheet(tr("background-color: rgb(255, 0, 0);"));
    }
}

void DocInfoEdit::on_lineEditUserNum_textChanged(QString str)
{
    if(str.length() <= 0)
    {
        this->ui->lineEditUserNum->setStyleSheet(tr("background-color: rgb(255, 0, 0);"));
    }
    else
    {
        this->ui->lineEditUserNum->setStyleSheet(tr(""));
    }
    this->changeOKButton();
}

void DocInfoEdit::on_buttonBox_accepted()
{

}

void DocInfoEdit::changeOKButton()
{
    if(this->ui->linEditName->text() == tr(""))
    {
        this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        return;
    }

    if(this->ui->lineEditDocNum->text() == tr(""))
    {
        this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        return;
    }

    if(this->ui->lineEditUserName->text() == tr(""))
    {
        this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        return;
    }

    if(this->ui->lineEditUserNum->text() == tr(""))
    {
        this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        return;
    }

    this->ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
}
