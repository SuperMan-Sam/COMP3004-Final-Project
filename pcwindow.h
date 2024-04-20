#ifndef PCWINDOW_H
#define PCWINDOW_H

#include <QWidget>
#include "qcustomplot.h"

class PCWindow : public QWidget
{
    Q_OBJECT

public:
    explicit PCWindow(QWidget *parent = nullptr);

private:
    QCustomPlot *customPlot;
};

#endif // PCWINDOW_H
