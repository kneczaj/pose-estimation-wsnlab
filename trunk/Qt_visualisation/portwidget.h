#ifndef PORTWIDGET_H
#define PORTWIDGET_H

#include <QWidget>

#include <Qt/qextserialport.h>
#include <Qt/qextserialenumerator.h>

namespace Ui {
    class PortWidget;
}

class PortWidget : public QWidget {
    Q_OBJECT
public:
    PortWidget(QWidget *parent = 0);
    ~PortWidget();

    QString getSelectedPort();

    void closePort();

signals:
    void portOpenError();
    void portOpenRequest(const QString &portName);
    void portCloseRequest();

public slots:
    void portOpened();
    void portClosed();

private slots:
    void openSelectedPort();

protected:
    void changeEvent(QEvent *e);

private:
    void addPorts(QList<QextPortInfo> ports);

    Ui::PortWidget *ui;
};

#endif // PORTWIDGET_H
