#include "aboutwindow.h"
#include "ui_aboutwindow.h"

#include <QDesktopWidget>

AboutWindow::AboutWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutWindow),
    systemInfo(NULL),
    docInfo(NULL),
    isShown(false)
{
    ui->setupUi(this);

    //设置窗口居中显示
    QDesktopWidget * desktop = QApplication::desktop();
    this->move((desktop->width() - this->width()) / 2,(desktop->height() - this->height()) / 2);
}

AboutWindow::~AboutWindow()
{
    delete ui;
}

void AboutWindow::showEvent(QShowEvent *e)
{
    if(!this->isShown)
    {
        this->ui->label_3->setText(tr("v") + this->systemInfo->version);
    }
}
