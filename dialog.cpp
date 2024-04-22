#include "dialog.h"
#include "ui_dialog.h"
#include "QDebug"
#include "mainwindow.h"

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
    qDebug() << "Clicked on item" << item->text();
    if (item) {
        QString title = item->text();
        MainWindow* mWindow = qobject_cast<MainWindow*>(parentWidget());
        if (mWindow) {
            mWindow->showLog(title); // Call showLog function of MainWindow
        } else {
            qDebug() << "mWindow is null!";
        }
    }
}

void Dialog::showLogs(const QString& title, const QString& data)
{
    ui->listWidget->addItem(title);
}

void Dialog::clearLogs(){
    ui->listWidget->clear();
}
