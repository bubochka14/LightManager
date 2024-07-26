#include "serialtransport.h"
Q_LOGGING_CATEGORY(LC_SERIAL,"SerialTransport")
SerialTransport::SerialTransport(QObject *parent)
    : ATransport(parent)
{
}
bool SerialTransport::openPort(const QString& port)
{
    _serial.setPortName(port);
    if(!_serial.open(QIODevice::ReadWrite))
    {
        qCCritical(LC_SERIAL) << "Cannot open serial port: " << _serial.error();
        _errorString = _serial.errorString();
        return false;
    }
    return true;
}
QString SerialTransport::errorString() const
{
    return _errorString;
}
int SerialTransport::write(const char* msg)
{
    return _serial.write(msg);
}
QStringList SerialTransport::availablePorts() const
{
    QStringList out;
    for (auto& info : QSerialPortInfo::availablePorts())
        out.push_back(info.portName());
    return out;
}
QString SerialTransport::openedPort() const
{
    return _serial.portName();
}
void SerialTransport::closePort()
{
    _serial.close();
}
