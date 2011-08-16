#ifndef SETTABLEALGIN_H
#define SETTABLEALGIN_H

#include <QDialog>

#include "yricheditor.h"

namespace Ui {
    class SetTableAlgin;
}

class SetTableAlgin : public QDialog
{
    Q_OBJECT

public:
    explicit SetTableAlgin(QWidget *parent = 0);
    ~SetTableAlgin();

    /*!
     \brief 获取表格对齐方式

     \return 对齐方式
    */
    YRichEditor::align getTableAlign();

    /*!
     \brief 设置表格对齐方式

     \param a 对齐方式
    */
    void setTableAlgin(YRichEditor::align a);
private:
    Ui::SetTableAlgin *ui;
};

#endif // SETTABLEALGIN_H
