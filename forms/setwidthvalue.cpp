#include "setwidthvalue.h"
#include "ui_setwidthvalue.h"

SetWidthValue::SetWidthValue(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SetWidthValue)
{
    ui->setupUi(this);
}

SetWidthValue::~SetWidthValue()
{
    delete ui;
}

void SetWidthValue::on_comboBox_currentIndexChanged(int index)
{
    if(0 == index)
    {
        this->ui->spinBox->setMaximum(1000);
    }
    else
    {
        this->ui->spinBox->setMaximum(100);
    }
}

int SetWidthValue::getValue()
{
    return this->ui->spinBox->value();
}

QTextLength::Type SetWidthValue::getValueType()
{
    if(0 == this->ui->comboBox->currentIndex())
    {
        return QTextLength::FixedLength;
    }
    else
    {
        return QTextLength::PercentageLength;
    }
}

void SetWidthValue::setValue(const int &v)
{
    this->ui->spinBox->setValue(v);
}

void SetWidthValue::setValueType(const QTextLength::Type &t)
{
    if(QTextLength::FixedLength == t)
    {
        this->ui->comboBox->setCurrentIndex(0);
    }
    else
    {
        this->ui->comboBox->setCurrentIndex(1);
    }
}
