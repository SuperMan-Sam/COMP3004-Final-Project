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

GraphData PCWindow::graphData() {
    int data = 58;
    QVector<double> x(data), y(data);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> amp(-1.0, 1.0);
    float distValue = (xMax - 1)/(data - 1);    // distribution value
    for(int i = 0; i < data; ++i) {
        x[i] = i * distValue;
        y[i] = amp(gen);
     }

    GraphData dataLog;
    dataLog.xData = x;
    dataLog.yData = y;
    qDebug() <<"X: " << dataLog.xData;
    qDebug() <<"Y: " << dataLog.yData;

    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    customPlot->replot();

    return dataLog;
}

void PCWindow::handleStatusChanged() {
    std::cout << "22222";
    GraphData data = graphData();
}
