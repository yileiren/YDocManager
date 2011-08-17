#ifndef SETWIDTHVALUE_H
#define SETWIDTHVALUE_H

#include <QDialog>
#include <QTextLength>

namespace Ui {
    class SetWidthValue;
}

class SetWidthValue : public QDialog
{
    Q_OBJECT

public:
    explicit SetWidthValue(QWidget *parent = 0);
    ~SetWidthValue();

    /*!
     \brief 获取数值

    */
    int getValue();

    /*!
     \brief 获取数值类型

    */
    QTextLength::Type getValueType();

    /*!
     \brief 设置数值

     \param v 数值
    */
    void setValue(const int &v);

    /*!
     \brief 设置数值类型

     \param t 类型
    */
    void setValueType(const QTextLength::Type &t);

private slots:
    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::SetWidthValue *ui;
};

#endif // SETWIDTHVALUE_H
