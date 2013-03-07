#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include <Qt/qextserialport.h>

#include "accelerometerwidget.h"
#include "receiver.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void setMoteIndicator(const int &mac, float xVal, float yVal, float battery);

protected:
    void changeEvent(QEvent *e);

private:
    enum Pose {sitting, standing, lying};

    Pose pose;

    Ui::MainWindow *ui;
    QMessageBox errorMessageBox;

    QextSerialPort *port;
    Receiver *receiver;

    void parsePacket(const QString &text);

    QHash<int, AccelerometerWidget*> motesMap;
    void setAccelerometerWidgetName(AccelerometerWidget *widget, const int mac);

    int thr3y;
    int thr2y;
    int thr1y;

private slots:

    void errorMessage(const QString &message);
    void openPortErrorMessage();

    void on_spinThr1_valueChanged(int val);
    void on_spinThr2_valueChanged(int val);
    void on_spinThr3_valueChanged(int val);
};

#endif // MAINWINDOW_H
