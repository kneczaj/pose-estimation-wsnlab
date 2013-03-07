#ifndef ACCELEROMETERWIDGET_H
#define ACCELEROMETERWIDGET_H

#include <QWidget>
#include <QtGui>
#include "indicator.h"

namespace Ui {
    class AccelerometerWidget;
}

class AccelerometerWidget : public QWidget {
    Q_OBJECT
public:
    AccelerometerWidget(QWidget *parent = 0);
    ~AccelerometerWidget();

    void setXValue(float val);
    void setYValue(float val);
    void setBatteryValue(float val);
    void setLabel(const QString &label);
    void setMaxValue(float val);

    float getX();
    float getY();

protected:
    void changeEvent(QEvent *e);

private:
    float x;
    float y;
    Ui::AccelerometerWidget *ui;
};

#endif // ACCELEROMETERWIDGET_H
