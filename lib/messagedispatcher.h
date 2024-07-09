#ifndef ATRANSPORT_H
#define ATRANSPORT_H
#include <QObject>
class MessageDispatcher: public QObject
{
    Q_OBJECT;
public:
    explicit MessageDispatcher(QObject* parent = nullptr);
    virtual bool postMessage(const char* msg);
    virtual ~MessageDispatcher();
public slots:
    virtual bool setUserIllumination(size_t illum);
    virtual bool setMode(const QString& other);
signals:
    void msgReceived(const QString& msg);
    void modeChanged(const QString&);
    void externalIlluminationChanged(int);
    void transferMessage(const char* msg);
};

#endif // ATRANSPORT_H
