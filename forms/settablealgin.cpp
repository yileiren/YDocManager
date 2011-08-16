#include "settablealgin.h"
#include "ui_settablealgin.h"

#include <QDesktopWidget>

SetTableAlgin::SetTableAlgin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetTableAlgin)
{
    ui->setupUi(this);

    //设置窗口居中显示
    QDesktopWidget * desktop = QApplication::desktop();
    this->move((desktop->width() - this->width()) / 2,(desktop->height() - this->height()) / 2);
}

SetTableAlgin::~SetTableAlgin()
{
    delete ui;
}

YRichEditor::align SetTableAlgin::getTableAlign()
{
    if(this->ui->radioButton->isChecked())
    {
        return YRichEditor::Left;
    }
    else if(this->ui->radioButton_2->isChecked())
    {
        return YRichEditor::Center;
    }
    else
    {
        return YRichEditor::Right;
    }
}

void SetTableAlgin::setTableAlgin(YRichEditor::align a)
{
    if(YRichEditor::Left == a)
    {
        this->ui->radioButton->setChecked(true);
        this->ui->radioButton_2->setChecked(false);
        this->ui->radioButton_3->setChecked(false);
    }
    else if(YRichEditor::Center == a)
    {
        this->ui->radioButton->setChecked(false);
        this->ui->radioButton_2->setChecked(true);
        this->ui->radioButton_3->setChecked(false);
    }
    else
    {
        this->ui->radioButton->setChecked(false);
        this->ui->radioButton_2->setChecked(false);
        this->ui->radioButton_3->setChecked(true);
    }
}
