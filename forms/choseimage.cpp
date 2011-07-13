#include "choseimage.h"
#include "ui_choseimage.h"

ChoseImage::ChoseImage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChoseImage),
    isShown(false)
{
    ui->setupUi(this);
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
        this->pixmap = new QPixmap(tr("C:/Users/Administrator/Pictures/EYE.jpg"));
        this->ui->label->setPixmap(*this->pixmap);
    }
}
