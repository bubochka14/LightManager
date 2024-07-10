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
    virtual bool postData(const char* msg);
    virtual bool setUserIllumination(double illum);
    virtual bool setMode(const QString& other);
signals:
    void modeChanged(const QString&);
    void sensorValueChanged(double);
    void transferData(const char* msg);
};

#endif // ATRANSPORT_H
