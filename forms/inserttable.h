#ifndef INSERTTABLE_H
#define INSERTTABLE_H

#include <QDialog>

namespace Ui {
    class InsertTable;
}

class InsertTable : public QDialog
{
    Q_OBJECT

public:
    explicit InsertTable(QWidget *parent = 0);
    ~InsertTable();

private:
    Ui::InsertTable *ui;
};

#endif // INSERTTABLE_H
