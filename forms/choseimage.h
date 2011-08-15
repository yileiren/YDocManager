#ifndef CHOSEIMAGE_H
#define CHOSEIMAGE_H

#include <QDialog>
#include <QListWidgetItem>

#include "ydocsyetem.h"


namespace Ui {
    class ChoseImage;
}

class ChoseImage : public QDialog
{
    Q_OBJECT

public:
    explicit ChoseImage(QWidget *parent = 0);
    ~ChoseImage();

    FileInfo *openingFile; /*!< 正在编辑的文档 */

    QString selectedText; /*!< 选中的图片名称 */

private slots:
    void on_buttonBox_accepted();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_listWidget_itemSelectionChanged();

    void on_pushButton_3_clicked();

private:
    Ui::ChoseImage *ui;

    bool isShown; /*!< 窗口是否已经显示 */

    //QListWidgetItem *

    /*!
     \brief 窗口显示事件

     \param e 事件
    */
    void showEvent(QShowEvent *e);

    /*!
     \brief 读取图片信息

    */
    void readImages();

    /*!
     \brief 写图片信息

    */
    void writeImages();
};

#endif // CHOSEIMAGE_H
