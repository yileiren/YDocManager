#ifndef SPLITCELLNUM_H
#define SPLITCELLNUM_H

#include <QDialog>

namespace Ui {
    class SplitCellNum;
}

class SplitCellNum : public QDialog
{
    Q_OBJECT

public:
    explicit SplitCellNum(QWidget *parent = 0);
    ~SplitCellNum();

    /*!
     \brief 获取拆分的行数

     \return 行数
    */
    int getRowCount();

    /*!
     \brief 获取拆分的列数

     \return 列数
    */
    int getColumnCount();
private:
    Ui::SplitCellNum *ui;
};

#endif // SPLITCELLNUM_H
