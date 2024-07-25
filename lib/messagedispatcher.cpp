#include "messagedispatcher.h"
using namespace Qt::Literals::StringLiterals;

MessageDispatcher::MessageDispatcher(QObject* parent)
    :QObject(parent)
    ,_sensor(0)
    ,_manual(0)
{}
MessageDispatcher::~MessageDispatcher(){}
void MessageDispatcher::requestMode()
{
    emit transferMessage("request:mode\n"_L1);
}
void MessageDispatcher::requestSensor()
{
    emit transferMessage("request:sensor\n"_L1);
}
void MessageDispatcher::setManual(double other)
{
    if (_manual == other)
        return;
    _manual = round(other * 100) / 100;
    _manual = _manual > 1 ? 1 : _manual;
    emit transferMessage("manual:%1\n"_L1.arg(QString::number(_manual)));
    emit manualChanged();
}
void MessageDispatcher::setMode(const QString& other)
{
    if (_mode == other)
        return;
    _mode = other;
    emit transferMessage("mode:%1\n"_L1.arg(_mode));
    emit modeChanged();
}
double MessageDispatcher::manual() const
{
    return _manual;
}
double MessageDispatcher::sensor() const
{
    return _sensor;
}
QString MessageDispatcher::mode() const
{
    return _mode;
}
void MessageDispatcher::handleMessage(const QString& message)
{
    _messageCache.append(message);
    QStringList messages = _messageCache.split(QRegularExpression("[\r\n]+"));
    _messageCache = messages.last();
    if (messages.size() <= 1)
        return;
    for (auto& message : messages)
    {
        QStringList entries = message.split(":", Qt::SkipEmptyParts);
        if (entries.size() != 2)
            continue;
        if (entries.first() == "sensor"_L1)
        {
            bool convert;
            double value = entries.at(1).toDouble(&convert);
            if (convert)
                setSensor(value);
        }
        if (entries.first() == "mode"_L1)
        {
            setMode(entries.at(1));
        }
    }
}
void MessageDispatcher::setSensor(double other)
{
    if (_sensor == other)
        return;
    _sensor = round(other * 100) / 100;
    _sensor = _sensor > 1 ? 1 : _sensor;
    emit sensorChanged();
}