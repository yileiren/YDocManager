#include "splitcellnum.h"
#include "ui_splitcellnum.h"

SplitCellNum::SplitCellNum(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SplitCellNum)
{
    ui->setupUi(this);
}

SplitCellNum::~SplitCellNum()
{
    delete ui;
}

int SplitCellNum::getRowCount()
{
    return this->ui->spinBox->value();
}

int SplitCellNum::getColumnCount()
{
    return this->ui->spinBox_2->value();
}
