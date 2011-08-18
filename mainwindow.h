#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ydocsyetem.h"
#include "appXML/docinfoxml.h"

#include <vector>

#include <QMainWindow>
#include <QTreeWidgetItem>
#include <QLabel>
#include <QTextCharFormat>
#include <QFontComboBox>
#include <QComboBox>

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
    void on_insertTableAction_triggered();
    void on_newDocAction_triggered();

    void on_openDocAction_triggered();

    void on_treeWidget_itemExpanded(QTreeWidgetItem* item);

    void on_treeWidget_itemCollapsed(QTreeWidgetItem* item);

    void on_treeWidget_itemSelectionChanged();

    void on_saveDocAction_triggered();

    void on_editDocAction_triggered();

    void on_closeDocAction_triggered();

    void on_setBoldAction_triggered();

    void on_undoAction_triggered();

    void on_redoAction_triggered();

    void on_setItalicAction_triggered();

    void on_setUnderlineAction_triggered();

    void on_yRichEditor_currentCharFormatChanged(QTextCharFormat format);

    void on_yRichEditor_textChanged();

    /**
     * @brief 设置字体方法
     *
     * @param f 字体
    */
    void changeTextFont(QString f);

    /**
     * @brief 设置字号
     *
     * @param s 字号
    */
    void changeSize(QString s);

    void changeTextStyle(int index);

    void on_setTextLeftAction_triggered();

    void on_setTextCenterAction_triggered();

    void on_setTextRightAction_triggered();

    void on_fontColorAction_triggered();

    void on_backgroundColorAction_triggered();

    void on_insertImageAction_triggered();

    void on_setTableAlignAction_triggered();

    void on_setTableBackGroundColorAction_triggered();

    void on_insertRowsAtTopAction_triggered();

    void on_insertRowsAtBottomAction_triggered();

    void on_insertColumnsAtBeginAction_triggered();

    void on_insertColumnsAtEndAction_triggered();

    void on_removeRowsAction_triggered();

    void on_removeColumnsAction_triggered();

    void on_setColumnsWidthAction_triggered();

    void on_mergeCellsAction_triggered();

    void on_splitCellAction_triggered();

    void on_setTableCellsPaddingAction_triggered();

    void on_setTableCellsSpacingAction_triggered();

    void on_setTableCellsBackGroundColorAction_triggered();

    void on_exitAction_triggered();



    void on_action_triggered();

private:
    Ui::MainWindow *ui;

    QFontComboBox *comboBox; /*!< 字体选择控件 */

    QComboBox *fontComBox; /*!< 字体选择控件 */

    QComboBox *comboSize; /*!< 字号设置 */

    QComboBox *comboBoxTextStyle; /*!< 文本样式控件 */

    FileInfo *openingFile; /*!< 正在编辑的文档 */

    bool isShow; /*!< 是否已经显示 */

    bool isChanged; /*!< 打开的文档是否修改 */

    QTreeWidgetItem * rootItem; /*!< 文档树的根节点 */

    QLabel statusLabel; /*!< 状态栏状态信息 */

    QLabel nameLabel; /*!< 在状态栏显示文档名称 */

    QLabel userNameLabel; /*!< 在状态栏显示用户名称 */

    /*!
     \brief 读取根节点，如果没有创建跟节点的XML自动创建空的XML

    */
    void createRootItem();

    /*!
     \brief 读取子节点

     \param parent 父节点
    */
    void readChildItem(QTreeWidgetItem * parent);

    /*!
     \brief 读二级节点

     \param parent 父节点
    */
    void readSeccendChildItem(QTreeWidgetItem *parent);

    /*!
     \brief 销毁子节点

     \param parent 父节点
    */
    void deleteChildItem(QTreeWidgetItem *parent);

    /*!
     \brief 销毁二级节点

     \param parent 父节点
    */
    void deleteSeccendChildItem(QTreeWidgetItem *parent);

    /*!
     \brief 将文档节点信息写入XML，该方法只写当前节点下一级所有节点的信息

     \param item 节点
     \return 成功返回true，否则返回false
    */
    bool writeDocInfoXML(const QTreeWidgetItem * item);

    /*!
     \brief 写入文档

     \param fileInfo 文档信息
     \return 成功返回true，否则返回false
    */
    bool writeDocFile(const FileInfo *fileInfo);

    /*!
     \brief 打开文档

     \param fileInfo 文档信息
     \return 成功返回true，否则返回false
    */
    bool openDocFile(const FileInfo *fileInfo);

    /*!
     \brief 是否允许打开选中的文档

     \return 成功返回true，否则返回false
    */
    bool docAlowOpen();

    /*!
     \brief 根据字体格式改变菜单状态

     \param f 字体
    */
    void changeMenuState(const QFont &f);

    /*!
     \brief 设置文本对齐方式按钮

    */
    void changeTextAligenAction();

    /*!
     \brief 显示关于窗口

    */
    void showAboutWindow();

    void showEvent(QShowEvent * e);

    virtual void resizeEvent(QResizeEvent *);

    virtual void closeEvent(QCloseEvent * e);
};

#endif // MAINWINDOW_H
