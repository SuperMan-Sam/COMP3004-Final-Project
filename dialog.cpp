#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::outputData(const QString &data)
{
    ui->listWidget->clear(); // Clear existing items
    QStringList lines = data.split("\n", Qt::SkipEmptyParts);
    for(const QString &line : lines)
    {
        ui->listWidget->addItem(line);
    }
}
void Dialog::on_listWidget_itemClicked(QListWidgetItem *item)
{
    std::cout << "1212";
}

