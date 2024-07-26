#ifndef SERIALTRANSPORT_H
#define SERIALTRANSPORT_H

#include <QObject>
#include <QQmlEngine>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <atransport.h>
#include <qloggingcategory.h>
Q_DECLARE_LOGGING_CATEGORY(LC_SERIAL);
class SerialTransport : public ATransport
{
    Q_OBJECT;
    QML_ELEMENT;
    Q_PROPERTY(QStringList availablePorts READ availablePorts NOTIFY availablePortsChanged)
    Q_PROPERTY(QString openedPort READ openedPort NOTIFY openedPortChanged);
public:
    explicit SerialTransport(QObject *parent = nullptr);
    virtual QStringList availablePorts() const;
    virtual QString openedPort() const;
    virtual QString errorString() const;
public slots:
    virtual bool openPort(const QString& port);
    virtual void closePort();
    virtual int write(const char* msg) override;
signals:
    void availablePortsChanged();
    void openedPortChanged();
private:
    QSerialPort _serial;
    QString _errorString;
};

#endif // SERIALTRANSPORT_H
