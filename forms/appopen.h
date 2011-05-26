#ifndef APPOPEN_H
#define APPOPEN_H

#include <QDialog>

namespace Ui {
    class appOpen;
}

/*!
 \brief 应用程序初始化窗口

*/
class AppOpen : public QDialog
{
    Q_OBJECT

public:
    explicit AppOpen(QWidget *parent = 0);
    ~AppOpen();

private:
    Ui::appOpen *ui;

    bool isShown; /*!< 窗体是否已经显示 */
    QTimer *timer; /*!< 启动窗体定时器 */
    /*!
     \brief 窗口显示事件，用来初始化系统

     \param e 事件
    */
    void paintEvent(QPaintEvent *e);

private slots:
    /*!
     \brief 系统初始化方法，窗口显示后自动调用

    */
    void initSystem();
};

#endif // APPOPEN_H
