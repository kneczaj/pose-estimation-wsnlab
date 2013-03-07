#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setAccelerometerWidgetName(ui->mote1, 1);
    setAccelerometerWidgetName(ui->mote2, 2);
    setAccelerometerWidgetName(ui->mote3, 3);

    ui->mote1->setMaxValue(500);
    ui->mote2->setMaxValue(500);
    ui->mote3->setMaxValue(500);

    // higher value means horzontal position
    thr3y = 410;
    thr2y = 480;
    thr1y = 320;

    ui->spinThr1->setValue(thr1y);
    ui->spinThr2->setValue(thr2y);
    ui->spinThr3->setValue(thr3y);

    receiver = new Receiver();

    connect(receiver, SIGNAL(portOpenError()), this, SLOT(openPortErrorMessage()));

    connect(ui->portWidget, SIGNAL(portOpenRequest(QString)), receiver, SLOT(openPort(QString)));
    connect(ui->portWidget, SIGNAL(portCloseRequest()), receiver, SLOT(closePort()));

    connect(receiver, SIGNAL(portOpened()), ui->portWidget, SLOT(portOpened()));
    connect(receiver, SIGNAL(portClosed()), ui->portWidget, SLOT(portClosed()));

    connect(receiver, SIGNAL(textReceived(QString)), ui->textOutput, SLOT(append(QString)));
    connect(receiver, SIGNAL(setMoteIndicator(int,float,float,float)), this, SLOT(setMoteIndicator(int,float,float,float)));

    connect(ui->spinThr1, SIGNAL(valueChanged(int)), this, SLOT(on_spinThr1_valueChanged(int)));
    connect(ui->spinThr2, SIGNAL(valueChanged(int)), this, SLOT(on_spinThr2_valueChanged(int)));
    connect(ui->spinThr3, SIGNAL(valueChanged(int)), this, SLOT(on_spinThr3_valueChanged(int)));
}

void MainWindow::on_spinThr1_valueChanged(int val)
{
    thr1y = val;
}

void MainWindow::on_spinThr2_valueChanged(int val)
{
    thr2y = val;
}

void MainWindow::on_spinThr3_valueChanged(int val)
{
    thr3y = val;
}

void MainWindow::setAccelerometerWidgetName(AccelerometerWidget *widget, const int mac)
{
    widget->setLabel(QString("%1").arg(mac));
    motesMap.insert(mac, widget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::openPortErrorMessage()
{
    errorMessage("Unable to open port!");
}

void MainWindow::errorMessage(const QString &message)
{
    errorMessageBox.setText(message);
    errorMessageBox.show();
}

void MainWindow::setMoteIndicator(const int &mac, float xVal, float yVal, float battery)
{
    AccelerometerWidget *mote = motesMap.value(mac);
    if (mote)
    {
        mote->setXValue(xVal);
        mote->setYValue(yVal);
        mote->setBatteryValue(battery);
    }

    if (ui->mote3->getY() > thr3y && ui->mote1->getY() > thr1y)
        if (ui->mote2->getY() > thr2y)
            ui->pose->setText("lying");
        else
            ui->pose->setText("crawling");
    else if (ui->mote2->getY() < thr2y && ui->mote1->getY() < thr1y)
        if (ui->mote3->getY() < thr3y)
            ui->pose->setText("standing");
        else
            ui->pose->setText("kneeling");
    else
        ui->pose->setText("sitting");
}
