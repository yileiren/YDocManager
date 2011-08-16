#include "inserttable.h"
#include "ui_inserttable.h"

#include <QDesktopWidget>
#include <QPushButton>
#include <QColorDialog>

InsertTable::InsertTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertTable)
{
    ui->setupUi(this);

    //设置窗口居中显示
    QDesktopWidget * desktop = QApplication::desktop();
    this->move((desktop->width() - this->width()) / 2,(desktop->height() - this->height()) / 2);
}

InsertTable::~InsertTable()
{
    delete ui;
}

void InsertTable::on_pushButton_clicked()
{
    //打开颜色选择对话框
    QColorDialog colorDlg(QColor(this->ui->R->value(),this->ui->G->value(),this->ui->B->value()),this);
    if(QDialog::Accepted ==  colorDlg.exec())
    {
        QColor color = colorDlg.selectedColor();
        int r,g,b;
        color.getRgb(&r,&g,&b);
        this->ui->R->setValue(r);
        this->ui->G->setValue(g);
        this->ui->B->setValue(b);
    }
}

int InsertTable::getRowCount()
{
    return this->ui->rowCount->value();
}

int InsertTable::getColoumCount()
{
    return this->ui->columnusCount->value();
}

int InsertTable::getCellsPadding()
{
    return this->ui->cellsPadding->value();
}

int InsertTable::getCellsSpacing()
{
    return this->ui->cellsSpacing->value();
}

QColor InsertTable::getBackgroundColor()
{
    return QColor(this->ui->R->value(),this->ui->G->value(),this->ui->B->value());
}

YRichEditor::align InsertTable::getTableAlign()
{
    if(this->ui->tableAlign->currentIndex() == 0)
    {
        return YRichEditor::Left;
    }
    else if(this->ui->tableAlign->currentIndex() == 1)
    {
        return YRichEditor::Center;
    }
    else
    {
        return YRichEditor::Right;
    }
}
