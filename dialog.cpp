#include "dialog.h"
#include "ui_dialog.h"
#include "unistd.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(ui->listWidget, &QListWidget::itemClicked, this, &Dialog::on_listWidget_itemClicked);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    // Handle item click here
    //qDebug() << "Clicked on item" << item->text();
}

void Dialog::showLogs(const QString& data)
{
    ui->listWidget->addItem(data);
}
