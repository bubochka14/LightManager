#include "serialtransport.h"

SerialTransport::SerialTransport(QObject *parent)
    : ATransport(parent)
{
}
bool SerialTransport::openPort(const QString& port)
{
    return false;
}
int SerialTransport::write(const char* msg)
{
    return 0;
}
QStringList SerialTransport::availablePorts() const
{
    return QStringList();
}
QString SerialTransport::openedPort() const
{
    return "";
}
void SerialTransport::closePort()
{

}
