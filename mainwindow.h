#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QLabel>
#include <QToolButton>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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

    float calculateBaseline(float Alpha, float Beta, float Delta, float Theta, float Gamma, float A1, float A2, float A3, float A4, float A5);
    float offsetBaseline(float fd, int round);

    void newSession();
    void sessionLog();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    int remainingSeconds;
    int powerLeft = 100;
    int powerConsumed = 0;
    int process = 0;
    bool startSignal = false;
    bool processSignal = false;
    bool contactSignal = false;

    float fd;   // baseline
    int round = 1;  // round

};

#endif // MAINWINDOW_H
