#include "messagedispatcher.h"
MessageDispatcher::MessageDispatcher(QObject* parent)
    :QObject(parent)
{}
MessageDispatcher::~MessageDispatcher(){}
bool MessageDispatcher::postMessage(const char* msg)
{
    return false;
}

bool MessageDispatcher::setUserIllumination(double illum)
{
    return false;
}
bool MessageDispatcher::setMode(const QString& other)
{
    return false;
}