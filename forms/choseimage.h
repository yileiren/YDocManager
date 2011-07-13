#ifndef CHOSEIMAGE_H
#define CHOSEIMAGE_H

#include <QDialog>
#include <QPixmap>

namespace Ui {
    class ChoseImage;
}

class ChoseImage : public QDialog
{
    Q_OBJECT

public:
    explicit ChoseImage(QWidget *parent = 0);
    ~ChoseImage();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ChoseImage *ui;

    bool isShown; /*!< 窗口是否已经显示 */

    QPixmap *pixmap;  /*!< 显示的图像 */

    /*!
     \brief 窗口显示事件

     \param e 事件
    */
    void showEvent(QShowEvent *e);
};

#endif // CHOSEIMAGE_H
