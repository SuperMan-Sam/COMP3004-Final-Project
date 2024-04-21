#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void outputData(const QString &data);

private slots:
    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
