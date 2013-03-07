#include <QRegExp>

#include "receiver.h"

Receiver::Receiver(QObject *parent) :
    QThread(parent)
{
}

void Receiver::run()
{
    process();
}

void Receiver::receive()
{
    if (!isRunning())
    {
        if (port.isOpen())
            start();
        else
            emit error("Port not set - cannot start processing");
    }
}

void Receiver::openPort(const QString &portName)
{
    portMutex.lock();
    port.setQueryMode(QextSerialPort::EventDriven);
    port.setPortName(portName);
    port.setBaudRate(BAUD115200);
    port.setFlowControl(FLOW_OFF);
    port.setParity(PAR_NONE);
    port.setDataBits(DATA_8);
    port.setStopBits(STOP_1);
    port.open(QIODevice::ReadWrite);

    if (!port.isOpen())
    {
        portMutex.unlock();
        emit portOpenError();
        return;
    }
    connect(&port, SIGNAL(readyRead()), this, SLOT(receive()));
    portMutex.unlock();
    emit portOpened();
}

void Receiver::closePort()
{
    portMutex.lock();
    if (port.isOpen())
        port.close();

    portMutex.unlock();
    disconnect(&port, SIGNAL(readyRead()), this, SLOT(receive()));
    emit portClosed();
}

void Receiver::process()
{
    QString str;
    char ch;
    portMutex.lock();
    while (port.getChar(&ch))
    {
        portMutex.unlock();
        str.append(ch);
        if (ch == '\n')     // End of line, start decoding
        {
            str.remove("\r\n", Qt::CaseSensitive);
            emit textReceived(str);

            parsePacket(str);

            str.clear();
        }
        portMutex.lock();
    }
    portMutex.unlock();
    exit();
}

void Receiver::parsePacket(const QString &text)
{
    // mesage format:
    // #N:_____;X:_____;Y:______;V:____

    QRegExp rx("R AC#senderMac:([^;]*);x:(\\d*\\.\\d+);y:(\\d*\\.\\d+);bat:(\\d*\\.\\d+)");
    int moteMac;
    float xVal, yVal, battery;
    bool okX = true;
    bool okY = true;
    bool okBatt = true;
    bool okMac = true;

    int pos = rx.indexIn(text);
    if (pos > -1)
    {
        moteMac = rx.cap(1).toInt(&okMac);
        xVal = rx.cap(2).toFloat(&okX);
        yVal = rx.cap(3).toFloat(&okY);
        battery = rx.cap(4).toFloat(&okBatt);
    }

    if (okX && okY && okBatt && okMac)
        emit setMoteIndicator(moteMac, xVal, yVal, battery);
}
