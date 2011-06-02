#ifndef EDITFILEINFO_H
#define EDITFILEINFO_H

#include "ydocsyetem.h"

#include <QDialog>

namespace Ui {
    class EditFileInfo;
}

class EditFileInfo : public QDialog
{
    Q_OBJECT

public:
    explicit EditFileInfo(QWidget *parent = 0);
    ~EditFileInfo();

    FileInfo *fileInfo; /*!< 文件信息 */
private slots:
    void on_radioButtonFile_clicked(bool checked);

    void on_radioButtonDir_clicked(bool checked);

    void on_lineEditTitle_textChanged(QString str);

    void on_lineEditTitle_editingFinished();

private:
    Ui::EditFileInfo *ui;

    bool isShown; /*!< 是否已经显示 */

    void showEvent(QShowEvent *e);
};

#endif // EDITFILEINFO_H
