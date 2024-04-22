#include "pcwindow.h"

PCWindow::PCWindow(QWidget *parent) : QWidget(parent), ui(new Ui::PCWindow)
{
    ui->setupUi(this);
}

void PCWindow::setGraph(int plot_id, const QVector<double> &xData, const QVector<double> &yData) {
    QVector<QCustomPlot*> plots = {
        ui->customPlot1,
        ui->customPlot2,
        ui->customPlot3,
        ui->customPlot4,
        ui->customPlot5,
        ui->customPlot6,
        ui->customPlot7
    };
    qDebug() << plot_id;
    qDebug() << xData;
    plots[plot_id]->clearGraphs();
    plots[plot_id]->addGraph();
    plots[plot_id]->graph(0)->setData(xData, yData);
    plots[plot_id]->xAxis->setRange(xMin, xMax);
    plots[plot_id]->yAxis->setRange(yMin, yMax);
    plots[plot_id]->replot();

    show();
}
