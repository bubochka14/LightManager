#include "app.h"
using namespace Qt::Literals::StringLiterals;

App::App(int argc, char **argv)
    :_qApp(std::make_shared<QGuiApplication>(argc,argv))
    ,_engine(std::make_shared<QQmlApplicationEngine>())
    ,_sett("Org"_L1,"Light Manager"_L1)
{


    _engine->load(QUrl("qrc:/Main.qml"_L1));
    _view  = _engine->rootObjects().isEmpty()?nullptr:_engine->rootObjects().first();
    if(!_view)
    {
        std::runtime_error("App window creation error");
    }
}

QQmlApplicationEngine * App::engine() const
{
    return _engine.get();
}

int App::exec()
{
    return _qApp->exec();
}
void App::exit(int code)
{
    _qApp->exit(code);
}
void App::setSerialTransport(std::shared_ptr <SerialTransport> other)
{
    _serial = other;
}
App::~App()
{
}


std::shared_ptr<SerialTransport> App::serialTransport() const
{
    return _serial;
}

void App::setMessageDispatcher(std::shared_ptr<MessageDispatcher> other)
{
    _disp = other;
}
std::shared_ptr<MessageDispatcher> App::messageDispatcher() const
{
    return _disp;
}