#include "accelerometerwidget.h"
#include "ui_accelerometerwidget.h"

AccelerometerWidget::AccelerometerWidget(QWidget *parent):
    QWidget(parent),
    ui(new Ui::AccelerometerWidget)
{
    ui->setupUi(this);

    ui->xIndicator->setLabel("X axis");
    ui->yIndicator->setLabel("Y axis");
    ui->batteryIndicator->setLabel("Battery");
    ui->batteryIndicator->setMaxValue(4);
}

void AccelerometerWidget::setMaxValue(float val)
{
    ui->xIndicator->setMaxValue(val);
    ui->yIndicator->setMaxValue(val);
}

void AccelerometerWidget::setLabel(const QString &label)
{
    ui->groupBox->setTitle(label);
}

void AccelerometerWidget::setXValue(float val)
{
    ui->xIndicator->setValue(val);
    x = val;
}

void AccelerometerWidget::setYValue(float val)
{
    ui->yIndicator->setValue(val);
    y = val;
}

float AccelerometerWidget::getX()
{
    return x;
}

float AccelerometerWidget::getY()
{
    return y;
}

void AccelerometerWidget::setBatteryValue(float val)
{
    ui->batteryIndicator->setValue(val);
}

AccelerometerWidget::~AccelerometerWidget()
{
}

void AccelerometerWidget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
