#include "Sensor.h"

Sensor::Sensor(QObject *parent) : QObject(parent) {
    getWaveRanges();
    getAmplitudeRanges();
}

WaveRanges Sensor::getWaveRanges() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> distAlpha(8.0, 12.0);
    std::uniform_real_distribution<double> distBeta(12.0, 30.0);
    std::uniform_real_distribution<double> distDelta(1.0, 4.0);
    std::uniform_real_distribution<double> distTheta(4.0, 8.0);
    std::uniform_real_distribution<double> distGamma(25.0, 100.0);

    WaveRanges ranges;
    ranges.Alpha = distAlpha(gen);
    ranges.Beta = distBeta(gen);
    ranges.Delta = distDelta(gen);
    ranges.Theta = distTheta(gen);
    ranges.Gamma = distGamma(gen);

    return ranges;
}

AmplitudeRanges Sensor::getAmplitudeRanges() {
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> distAmplitude(0.1, 5.0);

    AmplitudeRanges ranges;
    ranges.A1 = distAmplitude(gen);
    ranges.A2 = distAmplitude(gen);
    ranges.A3 = distAmplitude(gen);
    ranges.A4 = distAmplitude(gen);
    ranges.A5 = distAmplitude(gen);

    return ranges;
}


