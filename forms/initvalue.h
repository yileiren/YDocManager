#ifndef INITVALUE_H
#define INITVALUE_H

#include <QDialog>

namespace Ui {
    class InitValue;
}

class InitValue : public QDialog
{
    Q_OBJECT

public:
    explicit InitValue(QWidget *parent = 0);
    ~InitValue();

    /*!
     \brief 获取输入的数值

     \return 输入的数值
    */
    int getInputValue();

    /*!
     \brief 色绘制输入的数值

     \param i 数值
    */
    void setInputValue(const int &i);
private:
    Ui::InitValue *ui;
};

#endif // INITVALUE_H
