#ifndef INSERTTABLE_H
#define INSERTTABLE_H

#include <QDialog>

#include "yricheditor.h"

namespace Ui {
    class InsertTable;
}

class InsertTable : public QDialog
{
    Q_OBJECT

public:
    explicit InsertTable(QWidget *parent = 0);
    ~InsertTable();

    /*!
     \brief 获取行数

     \return 行数
    */
    int getRowCount();

    /*!
     \brief 获取列数

     \return 列数
    */
    int getColoumCount();

    /*!
     \brief 获取表格内边距

     \return 表格内边距
    */
    int getCellsPadding();

    /*!
     \brief 获取表格外边距

     \return 表格外边距
    */
    int getCellsSpacing();

    /*!
     \brief 获取表格对齐方式

     \return 表格对齐方式
    */
    YRichEditor::align getTableAlign();

    /*!
     \brief 获取表格背景色

     \return 表格背景色
    */
    QColor getBackgroundColor();
private slots:
    void on_pushButton_clicked();

private:
    Ui::InsertTable *ui;
};

#endif // INSERTTABLE_H
