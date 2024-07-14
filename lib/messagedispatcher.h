#ifndef ATRANSPORT_H
#define ATRANSPORT_H
#include <QObject>
class MessageDispatcher: public QObject
{
    Q_OBJECT;
public:
    explicit MessageDispatcher(QObject* parent = nullptr);
    virtual ~MessageDispatcher();
public slots:
    virtual bool postMessage(const char* msg);
    virtual bool setManualIllumination(double illum);
    virtual bool setMode(const QString& other);
signals:
    void modeChanged(const QString&);
    void sensorIlluminanceChanged(double);
    void transferMessage(const char* msg);
};

#endif // ATRANSPORT_H
