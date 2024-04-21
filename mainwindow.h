#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QToolButton>
#include <QDateTime>
#include <fstream>
#include <QFile>
#include <QTextStream>
#include <QCoreApplication>
#include <QDebug>

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
    float calculateBaseline(float Alpha, float Beta, float Delta, float Theta, float Gamma, float A1, float A2, float A3, float A4, float A5);
    float offsetBaseline(float fd, int round);

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
    QString getTime();

    GraphData graphData(int remainingSeconds);


    void newSession();
    void sessionLog();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    int remainingSeconds = 29;
    int powerLeft = 70;
    int powerConsumed = 0;
    int process = 0;
    bool startSignal = false;
    bool processSignal = false;
    bool contactSignal = false;
    // Define axies
    float xMin = 0.0;
    float xMax = 30.0;

    float yMin = -1.0;
    float yMax = 1.0;



};

#endif // MAINWINDOW_H
