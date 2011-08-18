#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

#include <QDialog>

#include <ydocsyetem.h>

namespace Ui {
    class AboutWindow;
}

class AboutWindow : public QDialog
{
    Q_OBJECT

public:
    explicit AboutWindow(QWidget *parent = 0);
    ~AboutWindow();

    SystemInfo *systemInfo; /*!< 系统信息 */
    DocInfo * docInfo; /*!< 文档信息 */

private:
    Ui::AboutWindow *ui;

    bool isShown;

    virtual void showEvent(QShowEvent *e);
};

#endif // ABOUTWINDOW_H
