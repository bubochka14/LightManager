#include "messagedispatcher.h"
using namespace Qt::Literals::StringLiterals;

MessageDispatcher::MessageDispatcher(QObject* parent)
    :QObject(parent)
{}
MessageDispatcher::~MessageDispatcher(){}
void MessageDispatcher::postMessage(const QString& msg)
{

    _messageCache.append(msg);
    QStringList messages = _messageCache.split(QRegularExpression("[\r\n]+"));
    _messageCache = messages.last();
    if (messages.size() == 1)
        return;
    for (auto& message : messages)
    {
        QStringList entries = message.split(":", Qt::SkipEmptyParts);
        if (entries.size() != 2)
            continue;
        if (entries.first() == "sensor"_L1)
        {
            bool convert;
            double value = round(entries.at(1).toDouble(&convert)*100)/100;
            if (convert)
            {
                emit sensorIlluminanceChanged(value > 1 ? 1 : value);
            }
        }
        if (entries.first() == "mode"_L1)
        {
            emit(modeChanged(entries.at(1)));
        }
    }
}
void MessageDispatcher::requestMode()
{
    emit transferMessage(QString("request:mode\n"));
}
void MessageDispatcher::requestSensorIlluminance()
{
    emit transferMessage(QString("request:sensor\n"));

}
void MessageDispatcher::setManualIllumination(double illum)
{
    illum = round(illum * 100) / 100;
    emit transferMessage(QString("manual:%1\n").arg(illum>1?1:illum));
}
void MessageDispatcher::setMode(const QString& other)
{
    emit transferMessage(QString("mode:%1\n").arg(other));
}