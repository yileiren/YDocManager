#ifndef DOCINFOEDIT_H
#define DOCINFOEDIT_H

#include "ydocsyetem.h"

#include <QDialog>

namespace Ui {
    class DocInfoEdit;
}

/*!
 \brief 文档信息编辑窗口

*/
class DocInfoEdit : public QDialog
{
    Q_OBJECT

public:
    explicit DocInfoEdit(QWidget *parent = 0);
    ~DocInfoEdit();

    DocInfo *docInfo;

private slots:


    /*!
     \brief 当失去焦点时将文本

    */
    void on_linEditName_editingFinished();

    void on_linEditName_textChanged(QString str);
    void on_lineEditDocNum_editingFinished();

    void on_lineEditDocNum_textChanged(QString str);

    void on_lineEditUserName_editingFinished();

    void on_lineEditUserName_textChanged(QString str);

    void on_lineEditUserNum_editingFinished();

    void on_lineEditUserNum_textChanged(QString str);

    void on_buttonBox_accepted();

private:
    Ui::DocInfoEdit *ui;

    bool isShown; /*!< 是否已经显示 */

    /*!
     \brief 窗口显示事件

     \param e
    */
    void showEvent(QShowEvent *e);

    /*!
     \brief 根据数据验证情况设置确定按钮状态

    */
    void changeOKButton();
};

#endif // DOCINFOEDIT_H
