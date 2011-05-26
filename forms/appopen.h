#ifndef APPOPEN_H
#define APPOPEN_H

#include "ydocsyetem.h"

#include <QDialog>
#include <QFile>

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

    /*!
     \brief 初始化状态枚举

    */
    enum initStateEnum
    {
        Ready = 0, /*!< 就绪 */
        NoInfoFile = -1, /*!< 没找到系统信息文件 */
        ReadInfoFileError = -2 /*!< 读取信息文件出错 */
    };

    explicit AppOpen(QWidget *parent = 0);
    ~AppOpen();

    initStateEnum initState; /*!< 应用程序初始化状态 */

    SystemInfo *systemInfo; /*!< 系统信息 */

private:
    Ui::appOpen *ui;

    bool isShown; /*!< 窗体是否已经显示 */
    QTimer *timer; /*!< 启动窗体定时器 */
    /*!
     \brief 窗口显示事件，用来初始化系统

     \param e 事件
    */
    void paintEvent(QPaintEvent *e);

    QFile infoFile; /*!< 系统信息文件 */

private slots:
    /*!
     \brief 系统初始化方法，窗口显示后自动调用

    */
    void initSystem();

    /*!
     \brief 创建系统信息文件

    */
    void createInfoFile();

    /*!
     \brief 读取系统信息

    */
    void readSystemInfo();
};

#endif // APPOPEN_H
