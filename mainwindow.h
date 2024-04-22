#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QList>
#include <QToolButton>
#include <QDateTime>
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDebug>
#include <QRegExp>
#include <QVector>
#include "dialog.h"
#include "Sensor.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

struct GraphData {
    QVector<double> xData;
    QVector<double> yData;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QString graphDataToString(const GraphData &data);
    void showLog(const QString& title);
    void showBaseline(const QString data);

    float fd;   // baseline

    int round = 1;
    int roundMax = 4;

    QString start_time;

private slots:
    void initializeTimer();
    void startTimer();
    void updateTime();
    void pauseTimer();
    void stopTimer();
    void updateProcessBar(int process);
    void showTime();
    void onContactSignalStateChanged(int state);
    void LED(bool contactSignal, bool processSignal);
    void showChargeLevel(int powerLeft);
    void saveLog(const QString& data);
    void showGraph();

    QString getTime();

    GraphData graphData(int elcapedSeconds, int process);
    GraphData parseGraphData(const QString& data);

    void updateGraphs(int elapsedSeconds, int process);


    void newSession();
    void sessionLog();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    int totalSeconds = 29;
    int elapsedSeconds = 0;
    int remainingSeconds = totalSeconds - elapsedSeconds;
    int powerLeft = 70;
    int powerConsumed = 0;
    int process = 0;

    bool startSignal = false;
    bool processSignal = false;
    bool contactSignal = false;
    // Define axies
    float xMin = 0.0;
    float xMax = 30.0;

    float yMin = -1.5;
    float yMax = 1.5;
    QMap<QString, QString> dataMap;
    static Dialog *globalDialog;

    int currentRound = 1;
    int printCounter;
    void printBaseline();
    float offsetBaseline(float fd, int round);
    float calculateBaseline(float Alpha, float Beta, float Delta, float Theta, float Gamma, float A1, float A2, float A3, float A4, float A5);

    WaveRanges waveData;
    AmplitudeRanges amplitudeData;
    QTimer *printTimer;
    Sensor *sensor;

};

#endif // MAINWINDOW_H
