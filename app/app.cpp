#include "app.h"
using namespace Qt::Literals::StringLiterals;
Q_LOGGING_CATEGORY(LC_APP, "Application");
App::App(int argc, char **argv)

{

}

QQmlApplicationEngine * App::engine() const
{
    return nullptr;
}

int App::exec()
{
    return 0;
}
void App::exit(int code)
{

}
void App::setSerialTransport(SerialTransport* other)
{
   
}
App::~App()
{
}


SerialTransport* App::serialTransport() const
{
    return nullptr;
}

void App::setMessageDispatcher(MessageDispatcher* other)
{

}
MessageDispatcher * App::messageDispatcher() const
{
    return nullptr;
}