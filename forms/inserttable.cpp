#include "inserttable.h"
#include "ui_inserttable.h"

InsertTable::InsertTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertTable)
{
    ui->setupUi(this);
}

InsertTable::~InsertTable()
{
    delete ui;
}
