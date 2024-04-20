#include "pcwindow.h"

PCWindow::PCWindow(QWidget *parent) : QWidget(parent)
{
    // Create the custom plot widget
    customPlot = new QCustomPlot(this);

    // Set up the plot layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(customPlot);
    setLayout(layout);

    // Set up the plot data
    QVector<double> xData, yData;
    // Add your data points here
    // Example:
    xData << 1 << 2 << 3 << 4 << 5 << 6 << 7;
    yData << 2 << 4 << 5 << 7 << 9 << 6 << 7;

    // Create a graph and set its data
    customPlot->addGraph();
    customPlot->graph(0)->setData(xData, yData);

    // Set up the axes
    customPlot->xAxis->setLabel("Time");
    customPlot->yAxis->setLabel("Value");

    // Set up the range of the axes (optional)
    customPlot->xAxis->setRange(0, 6);
    customPlot->yAxis->setRange(0, 10);

    // Replot the plot
    customPlot->replot();
}
