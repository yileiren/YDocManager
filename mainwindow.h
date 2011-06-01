#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ydocsyetem.h"
#include "appXML/docinfoxml.h"

#include <vector>

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QLabel>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    SystemInfo *systemInfo; /*!< 系统信息 */
    DocInfo * docInfo; /*!< 文档信息 */
private slots:
    void on_newDocAction_triggered();

private:
    Ui::MainWindow *ui;

    bool isShow; /*!< 是否已经显示 */

    QTreeWidgetItem * rootItem; /*!< 文档树的根节点 */

    QLabel statusLabel; /*!< 状态栏状态信息 */

    QLabel nameLabel; /*!< 在状态栏显示文档名称 */

    QLabel userNameLabel; /*!< 在状态栏显示用户名称 */

    /*!
     \brief 读取根节点，如果没有创建跟节点的XML自动创建空的XML

    */
    void createRootItem();

    /*!
     \brief 读取子节点，一次读取该节点下两层节点数据

     \param parent 父节点
    */
    void readChildItem(QTreeWidgetItem * parent);

    /*!
     \brief 将文档节点信息写入XML，该方法只写当前节点下一级所有节点的信息

     \param item 节点
    */
    bool writeDocInfoXML(QTreeWidgetItem * item);

    void paintEvent(QPaintEvent * e);

    void showEvent(QShowEvent * e);
};

#endif // MAINWINDOW_H
