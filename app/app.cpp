#include "app.h"
using namespace Qt::Literals::StringLiterals;
Q_LOGGING_CATEGORY(LC_APP, "Application");
App::App(int argc, char **argv)
    :_qApp(argc,argv)
    ,_serial(new SerialTransport(this))
    ,_disp(new MessageDispatcher(this))
    ,_sett("Org"_L1,"Light Manager"_L1)
    ,_engine(new QQmlApplicationEngine)
{
    qCDebug(LC_APP) << "creation ...";
    _engine->setInitialProperties({
        {"portNames",_serial->availablePorts()} });
    _engine->load(QUrl("qrc:/Main.qml"_L1));
    _view  = _engine->rootObjects().isEmpty()?nullptr:_engine->rootObjects().first();
    if(!_view)
    {
        qCCritical(LC_APP)<< "window creation error.";
    }
    else
    {
        connect(_view, SIGNAL(connectToPort(const QString&)),
            this, SLOT(handleConnect(const QString&)));
        qCDebug(LC_APP) << "created.";
    }
}
void App::handleConnect(const QString& port)
{
    _serial->openPort(port);
    _view->setProperty("pageState", "connecting");

}
quint32 App::connectionTimeout() const
{
    return _timeout;
}
void App::setConnectionTimeout(quint32 other)
{
    if (other == _timeout)
        return;
    _timeout = other;
    emit connectionTimeoutChanged();
}
QQmlApplicationEngine * App::engine() const
{
    return _engine;
}

int App::exec()
{
    if (!_view)
        return -1;
    qCDebug(LC_APP) << "executed.";
    return _qApp.exec();
}
void App::exit(int code)
{
    qCDebug(LC_APP) << "closed.";
    _qApp.exit(code);
}
void App::setSerialTransport(SerialTransport* other)
{
    if (other == _serial)
        return;
    _serial = other;
    _view->setProperty("portNames", _serial->availablePorts());
    emit serialTransportChanged();
}
App::~App()
{
}
SerialTransport* App::serialTransport() const
{
    return _serial;
}
void App::setMessageDispatcher(MessageDispatcher* other)
{
    if (_disp == other)
        return;
    _disp = other;
    emit messageDispatcherChanged();
}
MessageDispatcher * App::messageDispatcher() const
{
    return _disp;
}