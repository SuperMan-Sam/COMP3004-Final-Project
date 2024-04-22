#include "mainwindow.h"
#include "pcwindow.h"
#include "ui_mainwindow.h"

Dialog *MainWindow::globalDialog = nullptr;

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
    if (!globalDialog)
        globalDialog = new Dialog(this);

    sensor = new Sensor(this);
    waveData = sensor->getWaveRanges();
    amplitudeData = sensor->getAmplitudeRanges();

}

MainWindow::~MainWindow()
{
    delete ui;
}

// Initialize timer
void MainWindow::initializeTimer() {
    ui->label_Timer->setText("00:29");
    totalSeconds = 29;
    elapsedSeconds = 0;
    remainingSeconds = totalSeconds - elapsedSeconds;
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
        printTimer = new QTimer(this);
        connect(printTimer, &QTimer::timeout, this, &MainWindow::printBaseline);
        printTimer->start(1000/16);
    }
}

void MainWindow::updateTime() {
    if(contactSignal == true){
        if (remainingSeconds >= 0) {
            int minutes = remainingSeconds / 60;
            int seconds = remainingSeconds % 60;
            ui->label_Timer->setText(QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0')));

            // update process bar
            float percentage = static_cast<float>(elapsedSeconds) / static_cast<float>(totalSeconds);
            process = static_cast<int>(percentage * 100);
            updateProcessBar(process);

            elapsedSeconds++;
            remainingSeconds = totalSeconds - elapsedSeconds;
            powerLeft--;

            qDebug() <<"Remaining seconds: " << remainingSeconds;
            qDebug() <<"PowerLeft: " << powerLeft;

            showChargeLevel(powerLeft);
        }else {
            // Time is up, stop the timer
            timer->stop();
            showGraph();
            qDebug() << "Signal emitted: statusChanged";
            /*if(start_time != ""){
                saveLog(start_time + "-" + getTime()+ "/" + graphDataToString(datas));
                start_time.clear();
                std::cout << start_time.toStdString()+ "-" +getTime().toStdString()+ "/" + graphDataToString(datas).toStdString();
            }*/
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
    if(start_time != ""){
        showGraph();
    }
    initializeTimer();
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
        QSet<QString> uniqueTextBeforeSlash;
        for (const QString& line : lines) {
            int index1 = line.indexOf("/");
            if (index1 != -1) { // "=" found in the line
                QString textBeforeSlash = line.left(index1);
                QString textAfterSlash = line.right(line.length() - index1 - 1);
                if (!uniqueTextBeforeSlash.contains(textBeforeSlash)) {
                    globalDialog->showLogs(textBeforeSlash, textAfterSlash);
                    uniqueTextBeforeSlash.insert(textBeforeSlash); // Add to set to mark it as processed
                }
            }
            int index = line.indexOf("=");
            if (index != -1) { // "=" found in the line
                QString textBeforeSlash = line.left(index);
                QString textAfterSlash = line.right(line.length() - index - 1);
                dataMap.insert(textBeforeSlash, textAfterSlash);
            }
        }
        globalDialog->show();
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
    std::cout << data.toStdString() << std::endl;
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << data << "\n";
        file.close();
        std::cout << "Data appended to the file successfully.";
    } else {
        std::cout << "Failed to save file:";
    }
}

GraphData MainWindow::graphData(int elcapedSeconds, int process) {
    int data = 145;
    qDebug() <<"Current process: " << process;
    xMax = elcapedSeconds;     // time
    qDebug() <<"Current xMax: " << xMax;
    int numOfData = static_cast<int>(data * (static_cast<double>(process) / 100));      // num of data based on the process of time
    qDebug() <<"Num of Data: " << numOfData;
    QVector<double> x(numOfData), y(numOfData);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> amp(-1.0, 1.0);
    float distValue = (xMax)/(numOfData - 1);    // distribution value

    for(int i = 0; i < numOfData; ++i) {
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

QString MainWindow::graphDataToString(const GraphData &data) {
    QString result;

    // Iterate over xData and yData simultaneously
    for (int i = 0; i < data.xData.size() && i < data.yData.size(); ++i) {
        result += QString("(%1, %2), ").arg(data.xData[i]).arg(data.yData[i]);
    }

    // Remove the trailing comma and space
    if (!result.isEmpty()) {
        result.chop(2);
    }

    return result;
}

GraphData MainWindow::parseGraphData(const QString& data) {
    GraphData graphData;

    // Regular expression to match each pair of values
    QRegExp regex("\\((-?\\d+(?:\\.\\d+)?), (-?\\d+(?:\\.\\d+)?)\\)");

    int pos = 0;
    while ((pos = regex.indexIn(data, pos)) != -1) {
        QStringList values = regex.capturedTexts();
        if (values.size() >= 3) {
            bool ok;
            double x = values[1].toDouble(&ok);
            if (ok) graphData.xData.append(x);

            double y = values[2].toDouble(&ok);
            if (ok) graphData.yData.append(y);
        }
        pos += regex.matchedLength();
    }

    return graphData;
}


void MainWindow::showLog(const QString& title){
    //PCWindow *pc = new PCWindow();
    static std::unique_ptr<PCWindow> pc(new PCWindow());
    for (int i = 0; i < 7; ++i) {
        static char str[10];
        sprintf(str, "%d", i);
        qDebug() << title + "/" + str;
        GraphData data = parseGraphData(dataMap.value(title+ "/" + str));
        pc->setGraph(i,data.xData, data.yData);
    }
}

void MainWindow::showGraph(){
    GraphData data = graphData(elapsedSeconds, process);
    updateGraphs(elapsedSeconds, process);
}

void MainWindow::updateGraphs(int elapsedSeconds, int process) {

    for (int i = 0; i < 7; ++i) {
        GraphData data = graphData(elapsedSeconds, process);
        static std::unique_ptr<PCWindow> pc(new PCWindow());
        pc->setGraph(i, data.xData, data.yData);
        static char str[10]; // Assuming a maximum of 10 digits for the integer
        sprintf(str, "%d", i);
        saveLog(start_time + "-" + getTime()+ "/" + str + "=" + graphDataToString(data));
        if(start_time != ""){
            if(i==6){
                start_time.clear();
            }
        }
    }
}

void MainWindow::showBaseline(const QString data){
    ui->textBrowser->append(data);
}

void MainWindow::printBaseline() {
    if(contactSignal == false || startSignal == false){
        return;
    }
    if (currentRound <= roundMax && printCounter < 16) {
        float fd = calculateBaseline(waveData.Alpha, waveData.Beta, waveData.Delta, waveData.Theta, waveData.Gamma, amplitudeData.A1, amplitudeData.A2, amplitudeData.A3, amplitudeData.A4, amplitudeData.A5);
        fd = offsetBaseline(fd, currentRound);
        //std::cout << "Round " << currentRound << " Baseline: " << mw->fd << "Hz" << std::endl;
        QString baselineText = "Round " + QString::number(currentRound) + " Baseline: " + QString::number(fd) + "Hz";
        showBaseline(baselineText);
        printCounter++;
    } else if (printCounter >= 16) {
        printCounter = 0;
        currentRound++;
        if (currentRound > roundMax) {
            printTimer->stop();
            currentRound = 1;
            qDebug() << "10";
        }
    }
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

// calculateBaseline
float MainWindow::calculateBaseline(float Alpha, float Beta, float Delta, float Theta, float Gamma, float A1, float A2, float A3, float A4, float A5) {
    float fd = (Alpha * std::pow(A1, 2) + Beta * std::pow(A2, 2) + Delta * std::pow(A3, 2) + Theta * std::pow(A4, 2) + Gamma * std::pow(A5, 2))/(std::pow(A1, 2) + std::pow(A2, 2) + std::pow(A3, 2) + std::pow(A4, 2) + std::pow(A5, 2));
    return fd;
}
