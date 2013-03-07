#include "indicator.h"
#include "ui_indicator.h"

Indicator::Indicator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Indicator)
{
    ui->setupUi(this);
    maxValue = 10.0;
}

Indicator::~Indicator()
{
    delete ui;
}

void Indicator::setMaxValue(float val)
{
    maxValue = val;
}

void Indicator::setValue(float val)
{
    ui->lcd->display(val);
    if (val > maxValue)
        ui->bar->setValue(ui->bar->maximum());
    else
        ui->bar->setValue(val*(ui->bar->maximum()-ui->bar->minimum())/maxValue);
}

void Indicator::setLabel(const QString &label)
{
    ui->label->setText(label);
}

void Indicator::changeEvent(QEvent *e)
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
