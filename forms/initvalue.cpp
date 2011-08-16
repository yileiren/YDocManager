#include "initvalue.h"
#include "ui_initvalue.h"

#include <QDesktopWidget>

InitValue::InitValue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InitValue)
{
    ui->setupUi(this);

    //设置窗口居中显示
    QDesktopWidget * desktop = QApplication::desktop();
    this->move((desktop->width() - this->width()) / 2,(desktop->height() - this->height()) / 2);
}

InitValue::~InitValue()
{
    delete ui;
}

int InitValue::getInputValue()
{
    return this->ui->spinBox->value();
}

void InitValue::setInputValue(const int &i)
{
    this->ui->spinBox->setValue(i);
}
