#ifndef APP_H
#define APP_H
#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "serialtransport.h"
#include "messagedispatcher.h"
#include "qloggingcategory.h"
Q_DECLARE_LOGGING_CATEGORY(LC_APP);
class App : public QObject
{
    Q_OBJECT;
public:
    explicit App(int argc, char **argv);
    int exec();
    void exit(int code=0);
public:
    QQmlApplicationEngine * engine() const;
    void setSerialTransport(SerialTransport* other);
    SerialTransport* serialTransport() const;
    void setMessageDispatcher(MessageDispatcher* other);
    MessageDispatcher* messageDispatcher() const;
    virtual ~App();
};

#endif // APP_H
