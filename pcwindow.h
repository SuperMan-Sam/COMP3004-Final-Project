#ifndef PCWINDOW_H
#define PCWINDOW_H

#include <QWidget>
#include <QTimer>
#include <QVector>
#include "qcustomplot.h"
#include "Sensor.h"
#include "mainwindow.h"
#include "ui_pcwindow.h"

class PCWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PCWindow(QWidget *parent = nullptr);
    void setGraph(int plot_id, const QVector<double> &xData, const QVector<double> &yData);
    void updateGraph();


private:
    Ui::PCWindow *ui;
    QCustomPlot *customPlot;
    QTimer *graphTimer;


    GraphData graphData();

    int graphCounter;
    int printCounter;
    int currentRound = 1;

    int roundMax = 4;

    // Define axies
    float xMin = 0.0;
    float xMax = 30.0;

    float yMin = -1.5;
    float yMax = 1.5;

};

#endif // PCWINDOW_H
