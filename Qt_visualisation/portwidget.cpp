#include "portwidget.h"
#include "ui_portwidget.h"

#include <Qt/qextserialenumerator.h>
#include <Qt/qextserialport.h>

PortWidget::PortWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PortWidget)
{    
    ui->setupUi(this);

    // Get available COM Ports
    QList<QextPortInfo> ports = QextSerialEnumerator::getPorts();
    addPorts(ports);

    connect(this->ui->openButton, SIGNAL(clicked()), this, SLOT(openSelectedPort()));
    connect(this->ui->closeButton, SIGNAL(clicked()), this, SIGNAL(portCloseRequest()));
}

PortWidget::~PortWidget()
{
    delete ui;
}

void PortWidget::addPorts(QList<QextPortInfo> ports)
{
    for (int i = 0; i < ports.size(); i++)
        ui->interfaceSelector->addItem(ports.at(i).portName.toLocal8Bit().constData());
}

QString PortWidget::getSelectedPort()
{
    return "/dev/" + ui->interfaceSelector->currentText();
}

void PortWidget::openSelectedPort()
{
    emit portOpenRequest(getSelectedPort());
}

void PortWidget::portOpened()
{
    ui->closeButton->setEnabled(true);
    ui->openButton->setEnabled(false);
    ui->interfaceSelector->setEnabled(false);
}

void PortWidget::portClosed()
{
    ui->closeButton->setEnabled(false);
    ui->openButton->setEnabled(true);
    ui->interfaceSelector->setEnabled(true);
}

void PortWidget::changeEvent(QEvent *e)
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
