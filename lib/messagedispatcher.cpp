#include "messagedispatcher.h"
MessageDispatcher::MessageDispatcher(QObject* parent)
    :QObject(parent)
{}
MessageDispatcher::~MessageDispatcher(){}
void MessageDispatcher::postMessage(const QString& msg)
{
}
void MessageDispatcher::requestMode()
{
}
void MessageDispatcher::requestSensorIlluminance()
{

}
void MessageDispatcher::setManualIllumination(double illum)
{
}
void MessageDispatcher::setMode(const QString& other)
{
}