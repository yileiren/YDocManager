#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    isShow(false)
{
    ui->setupUi(this);

    //设置窗口居中显示
    QDesktopWidget * desktop = QApplication::desktop();
    this->move((desktop->width() - this->width()) / 2,(desktop->height() - this->height()) / 2);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readChildItem(QTreeWidgetItem *parent)
{}

void MainWindow::on_newDocAction_triggered()
{

}

void MainWindow::paintEvent(QPaintEvent *e)
{
    //窗体重绘时自动改变控件
    this->statusLabel.setMinimumSize(this->ui->statusBar->width() - 300,10);
    this->statusLabel.setMaximumSize(this->ui->statusBar->width() - 300,10);
}

void MainWindow::showEvent(QShowEvent *e)
{
    if(!this->isShow)
    {
        //创建状态显示设置状态显示Label
        this->statusLabel.setParent(this->ui->statusBar);
        this->statusLabel.setText(tr("就绪"));
        this->ui->statusBar->addWidget(&this->statusLabel);

        //显示文档名称
        this->nameLabel.setParent(this->ui->statusBar);
        this->nameLabel.setText(tr("文档名称：") + this->docInfo->name);
        this->nameLabel.setMinimumSize(130,10);
        this->ui->statusBar->addWidget(&this->nameLabel);

        //显示用户名称
        this->userNameLabel.setParent(this->ui->statusBar);
        this->userNameLabel.setText(tr("用户名称：") + this->docInfo->userName);
        this->userNameLabel.setMinimumSize(130,10);
        this->ui->statusBar->addWidget(&this->userNameLabel);
    }
}
