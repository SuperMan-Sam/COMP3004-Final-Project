#ifndef SENSOR_H
#define SENSOR_H

#include <QObject>
#include <QMap>
#include <iostream>
#include <QRandomGenerator>
#include <cmath>

struct WaveRanges {
    double Alpha, Beta, Delta, Theta, Gamma;
};

struct AmplitudeRanges {
    double A1, A2, A3, A4, A5;
};


class Sensor : public QObject {
    Q_OBJECT

public:
    explicit Sensor(QObject *parent = nullptr);
    WaveRanges getWaveRanges();
    AmplitudeRanges getAmplitudeRanges();

private:



};

#endif // SENSOR_H
