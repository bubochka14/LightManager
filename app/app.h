#ifndef APP_H
#define APP_H
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QSettings>
#include "serialtransport.h"
#include "messagedispatcher.h"
#include "qloggingcategory.h"
Q_DECLARE_LOGGING_CATEGORY(LC_APP);
class App : public QObject
{
    Q_OBJECT;
    Q_PROPERTY(quint32 connectionTimeout READ connectionTimeout WRITE setConnectionTimeout NOTIFY connectionTimeoutChanged);
    Q_PROPERTY(SerialTransport* serialTransport READ serialTransport WRITE setSerialTransport NOTIFY serialTransportChanged);
    Q_PROPERTY(MessageDispatcher* messageDispatcher READ messageDispatcher WRITE setMessageDispatcher NOTIFY messageDispatcherChanged);
public:
    explicit App(int argc, char **argv);
    int exec();
    void exit(int code=0);
public:
    QQmlApplicationEngine * engine() const;
    quint32 connectionTimeout() const;
    void setConnectionTimeout(quint32 other);
    void setSerialTransport(SerialTransport* other);
    SerialTransport* serialTransport() const;
    void setMessageDispatcher(MessageDispatcher* other);
    MessageDispatcher* messageDispatcher() const;
    virtual ~App();
signals:
    void connectionTimeoutChanged();
    void serialTransportChanged();
    void messageDispatcherChanged();
protected slots:
    void handleConnect(const QString& port);
private:
    QGuiApplication _qApp;
    QQmlApplicationEngine* _engine;
    SerialTransport* _serial;
    MessageDispatcher* _disp;
    QObject * _view;
    quint32 _timeout;
    QSettings _sett;
};

#endif // APP_H
