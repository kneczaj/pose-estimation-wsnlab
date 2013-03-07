#ifndef INDICATOR_H
#define INDICATOR_H

#include <QWidget>

namespace Ui {
    class Indicator;
}

class Indicator : public QWidget {
    Q_OBJECT
public:
    Indicator(QWidget *parent = 0);
    ~Indicator();

    void setValue(float val);
    void setMaxValue(float val);
    void setLabel(const QString &label);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::Indicator *ui;
    float maxValue;
};

#endif // INDICATOR_H
