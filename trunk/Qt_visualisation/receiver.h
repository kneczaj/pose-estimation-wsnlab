#ifndef RECEIVER_H
#define RECEIVER_H

#include <QThread>
#include <QQueue>

#include <Qt/qextserialport.h>

class Receiver : public QThread
{
Q_OBJECT
public:
    explicit Receiver(QObject *parent = 0);

protected:
    void run();

signals:
    void setMoteIndicator(const int &mac, float xVal, float yVal, float battery);
    void error(const QString &error);
    void textReceived(const QString &text);
    void portOpened();
    void portClosed();
    void portOpenError();

public slots:
//    void setPort(QextSerialPort *port);
    void receive();
    void openPort(const QString &portName);
    void closePort();

private:
    void parsePacket(const QString &text);
    void process();

    QextSerialPort port;

    QQueue<QString> queue;
    QMutex portMutex;
};

#endif // RECEIVER_H
