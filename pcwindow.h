#ifndef PCWINDOW_H
#define PCWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include "qcustomplot.h"
#include "Sensor.h"
#include "mainwindow.h"

struct GraphData {
    QVector<double> xData;
    QVector<double> yData;
};

class PCWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PCWindow(QWidget *parent = nullptr);

public slots:
    void handleStatusChanged();

private:
    QCustomPlot *customPlot;
    QTimer *printTimer;
    QTimer *graphTimer;


    MainWindow *mw;
    Sensor *sensor;

    WaveRanges waveData;
    AmplitudeRanges amplitudeData;

    void printBaseline();

    GraphData graphData();

    int graphCounter;
    int printCounter;
    int currentRound = 1;

    // Define axies
    float xMin = 0.0;
    float xMax = 30.0;

    float yMin = -1.0;
    float yMax = 1.0;

};

#endif // PCWINDOW_H
