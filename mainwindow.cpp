#include "mainwindow.h"
#include "Sensor.h"
#include "dialog.h"
#include "pcwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textBrowser->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    ui->label_Battery->setVisible(false);

    // Connect clicked signal to slot
    connect(ui->ContactSingalBox, &QCheckBox::stateChanged, this, &MainWindow::onContactSignalStateChanged);

    connect(ui->Quit_Btn, &QToolButton::clicked, qApp, &QApplication::quit);
    connect(ui->Start_Btn, &QToolButton::clicked, this, &MainWindow::startTimer);
    connect(ui->Pause_Btn, &QToolButton::clicked, this, &MainWindow::pauseTimer);
    connect(ui->Stop_Btn, &QToolButton::clicked, this, &MainWindow::stopTimer);
    connect(ui->CurrentTime_Btn, &QPushButton::clicked, this, &MainWindow::showTime);

    connect(ui->newSession_Btn, &QPushButton::clicked, this, &MainWindow::newSession);
    connect(ui->sessionLog_Btn, &QPushButton::clicked, this, &MainWindow::sessionLog);

    initializeTimer();
    LED(contactSignal, processSignal);

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Initialize timer
void MainWindow::initializeTimer() {
    ui->label_Timer->setText("00:29");
    remainingSeconds = 29;
    process = 0;
    updateProcessBar(process);
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
}

void MainWindow::startTimer() {
    if(contactSignal == true){
        if(remainingSeconds != 0 && startSignal == true){
            return;
        }
        startSignal = true;
        LED(contactSignal, true);
        updateTime(); // Update the label_Timer immediately
        timer->start(1000);
        start_time = getTime();
    }
}

void MainWindow::updateTime() {
    if(contactSignal == true){
        if (remainingSeconds >= 0) {
            int minutes = remainingSeconds / 60;
            int seconds = remainingSeconds % 60;
            ui->label_Timer->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));

            // update process bar
            process = static_cast<int>(static_cast<float>((29.0 - remainingSeconds) / 29.0) * 100);
            updateProcessBar(process);

            remainingSeconds--;
            powerLeft--;

            qDebug() <<"Remaining seconds: " << remainingSeconds;
            qDebug() <<"PowerLeft: " << powerLeft;

            showChargeLevel(powerLeft);
        }else {
            // Time is up, stop the timer
            timer->stop();
            qDebug() <<"NOW remaining seconds::::::::::::::::::::::::Remaining seconds: " << remainingSeconds;
            PCWindow *pc = new PCWindow();
            GraphData datas = graphData(remainingSeconds);
            pc->setGraph(datas.xData, datas.yData);
            pc->show();
            qDebug() << "Signal emitted: statusChanged";
            if(start_time != ""){
                saveLog(start_time + "-" + getTime());
                start_time.clear();
                std::cout << start_time.toStdString();
            }
        }
    }else {
        // disconnect
        timer->stop();
        LED(contactSignal, false);

    }
}

void MainWindow::pauseTimer() {
    // update LED_Green status
    LED(contactSignal, false);
    startSignal = false;
    timer->stop();

}

void MainWindow::stopTimer(){
    // update LED_Green status
    LED(contactSignal, false);
    startSignal = false;
    timer->stop();
    initializeTimer();
    if(start_time != ""){
        saveLog(start_time + "-" + getTime());
        start_time.clear();
        std::cout << start_time.toStdString();
        PCWindow *pc = new PCWindow();
        GraphData datas = graphData(remainingSeconds);
        pc->setGraph(datas.xData, datas.yData);
        pc->show();
    }
}

void MainWindow:: updateProcessBar(int process) {
    ui->progressBar->setMinimum(0);
    ui->progressBar->setMaximum(100);
    ui->progressBar->setValue(process);
}

// showCurrentTime
void MainWindow::showTime() {
    ui->textBrowser->append(getTime());
}

QString MainWindow::getTime() {
    QDateTime currentTime = QDateTime::currentDateTime();
    return currentTime.toString("yyyy-MM-dd-HH:mm:ss");
}

// onContactSignalStateChanged
void MainWindow::onContactSignalStateChanged(int state)
{

    // if connected
    if(state == Qt::Checked){
        contactSignal = true;
    }

    // if not connected
    if(state == Qt::Unchecked){
        startSignal = false;
        contactSignal = false;
    }

    LED(contactSignal, processSignal);
}

// LED
void MainWindow::LED(bool contactSignal, bool pSignal)
{
    // LED_Blue
    processSignal = pSignal;
    if (contactSignal == true) {
        QPixmap pixmap(":/new/prefix1/connection_On.png");
        ui->LED_Blue->setPixmap(pixmap.scaled(ui->LED_Blue->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else{
        QPixmap pixmap(":/new/prefix1/connection_Off.png");
        ui->LED_Blue->setPixmap(pixmap.scaled(ui->LED_Blue->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // LED_Red
    if (contactSignal == false){
        QPixmap pixmap(":/new/prefix1/disconnection_On.png");
        ui->LED_Red->setPixmap(pixmap.scaled(ui->LED_Red->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else{
        QPixmap pixmap(":/new/prefix1/disconnection_Off.png");
        ui->LED_Red->setPixmap(pixmap.scaled(ui->LED_Red->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }

    // LED_Grren
    if (contactSignal == true && processSignal == true){
        QPixmap pixmap(":/new/prefix1/dataProcessing_On.png");
        ui->LED_Green->setPixmap(pixmap.scaled(ui->LED_Red->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else{
        QPixmap pixmap(":/new/prefix1/dataProcessing_Off.png");
        ui->LED_Green->setPixmap(pixmap.scaled(ui->LED_Red->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
}

// showChargeLevel
void MainWindow::showChargeLevel(int powerLeft){
    if (powerLeft < 1){
        stopTimer();
        ui->label_Battery->setVisible(true);
    }
}

// calculateBaseline
float MainWindow::calculateBaseline(float Alpha, float Beta, float Delta, float Theta, float Gamma, float A1, float A2, float A3, float A4, float A5) {
    fd = (Alpha * std::pow(A1, 2) + Beta * std::pow(A2, 2) + Delta * std::pow(A3, 2) + Theta * std::pow(A4, 2) + Gamma * std::pow(A5, 2))/(std::pow(A1, 2) + std::pow(A2, 2) + std::pow(A3, 2) + std::pow(A4, 2) + std::pow(A5, 2));
    return fd;
}

// offsetBaseline
float MainWindow::offsetBaseline(float fd, int round) {
    switch (round) {
            case 1:
                fd += 5;
                break;
            case 2:
                fd += 10;
                break;
            case 3:
                fd += 15;
                break;
            case 4:
                fd += 20;
                break;
            default:
                std::cout << "Invalid round value" << std::endl;
                break;
        }
    return fd;
}

// newSession
void MainWindow::newSession(){
    stopTimer();
}

// sessionLog
void MainWindow::sessionLog() {
    QString filePath = QCoreApplication::applicationDirPath() + "/data.txt";
    QFile file(filePath); // File name to read from
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&file);
        QString fileContent = in.readAll(); // Read all content as a single QString
        QStringList lines = fileContent.split("\n", Qt::SkipEmptyParts); // Split content into lines
        Dialog dialogs;
        for (const QString& line : lines) {
            dialogs.showLogs(line);
        }
        dialogs.exec();
        file.close();
    } else {
        std::cout << "Failed to open file for reading.";
    }
}


void MainWindow::saveLog(const QString& data){

    QString filePath = QCoreApplication::applicationDirPath() + "/data.txt";
    QFile file(filePath); // File name to save to
    if (!file.exists()) { // Check if file doesn't exist
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            std::cerr << "Failed to create file.";
            return;
        }
        file.close();
    }
    std::cout << filePath.toStdString() << std::endl;
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << data << "\n";
        file.close();
        std::cout << "Data appended to the file successfully.";
    } else {
        std::cout << "Failed to save file:";
    }
}

GraphData MainWindow::graphData(int remainingSeconds) {
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

    /*customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);
    customPlot->replot();*/

    return dataLog;
}
