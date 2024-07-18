#ifndef ATRANSPORT_H
#define ATRANSPORT_H
#include <QObject>
#include <QString>

class MessageDispatcher: public QObject
{
    Q_OBJECT;
public:
    explicit MessageDispatcher(QObject* parent = nullptr);
    virtual ~MessageDispatcher();
public slots:
    virtual void postMessage(const QString& );
    virtual void setManualIllumination(double illum);
    virtual void setMode(const QString& other);
    virtual void requestMode();
    virtual void requestSensorIlluminance();
signals:
    void modeChanged(const QString&);
    void sensorIlluminanceChanged(double);
    void transferMessage(const QString&);
private:
};

#endif // ATRANSPORT_H
