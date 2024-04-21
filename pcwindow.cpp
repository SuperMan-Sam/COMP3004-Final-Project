#include "pcwindow.h"

PCWindow::PCWindow(QWidget *parent) : QWidget(parent)
{
    // Create the custom plot widget
    customPlot = new QCustomPlot(this);

    // Set up the plot layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(customPlot);
    setLayout(layout);

    customPlot->xAxis->setRange(xMin, xMax);
    customPlot->yAxis->setRange(-1, 1);

    // Get data
    mw = new MainWindow(this);
    sensor = new Sensor(this);

    waveData = sensor->getWaveRanges();
    amplitudeData = sensor->getAmplitudeRanges();

    printTimer = new QTimer(this);
    connect(printTimer, &QTimer::timeout, this, &PCWindow::printBaseline);
    printTimer->start(1000/16);
}

void PCWindow::printBaseline() {
    if (currentRound <= mw->roundMax && printCounter < 16) {
        mw->fd = mw->calculateBaseline(waveData.Alpha, waveData.Beta, waveData.Delta, waveData.Theta, waveData.Gamma, amplitudeData.A1, amplitudeData.A2, amplitudeData.A3, amplitudeData.A4, amplitudeData.A5);
        mw->fd = mw->offsetBaseline(mw->fd, currentRound);
        //std::cout << "Round " << currentRound << " Baseline: " << mw->fd << "Hz" << std::endl;
        printCounter++;
    } else if (printCounter >= 16) {
        printCounter = 0;
        currentRound++;
        if (currentRound > mw->roundMax) {
            printTimer->stop();
        }
    }
}

void PCWindow::setGraph(const QVector<double> &xData, const QVector<double> &yData) {
    std::cout << "22222";
    customPlot->addGraph();
    customPlot->graph(0)->setData(xData, yData);
    customPlot->replot();
}
