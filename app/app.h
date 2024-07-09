#ifndef APP_H
#define APP_H
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QSettings>
#include "serialtransport.h"
#include "messagedispatcher.h"
class App
{
public:
    explicit App(int argc, char **argv);
    int exec();
    void exit(int code=0);
public:
    QQmlApplicationEngine * engine() const;
    void setSerialTransport(std::shared_ptr<SerialTransport> other);
    std::shared_ptr<SerialTransport> serialTransport() const;

    void setMessageDispatcher(std::shared_ptr<MessageDispatcher> other);
    std::shared_ptr<MessageDispatcher> messageDispatcher() const;
    ~App();
private:
    std::shared_ptr<QGuiApplication> _qApp;
    std::shared_ptr<QQmlApplicationEngine> _engine;
    std::shared_ptr<SerialTransport> _serial;
    std::shared_ptr<MessageDispatcher> _disp;
    QObject * _view;
    QSettings _sett;
};

#endif // APP_H
