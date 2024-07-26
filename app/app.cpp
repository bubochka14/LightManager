#include "app.h"
using namespace Qt::Literals::StringLiterals;
Q_LOGGING_CATEGORY(LC_APP, "Application");
App::App(int argc, char **argv)
    :_qApp(argc,argv)
    ,_serial(nullptr)
    ,_disp(nullptr)
    ,_sett("Org"_L1,"Light Manager"_L1)
    ,_engine(new QQmlApplicationEngine(this))
    ,_timeout(2000)
{
    qCDebug(LC_APP) << "creation ...";
    _engine->load(QUrl("qrc:/Main.qml"_L1));
    _view = qobject_cast<QQuickWindow*>(_engine->rootObjects().isEmpty() ? nullptr : _engine->rootObjects().first());
    if(!_view)
    {
        qCCritical(LC_APP)<< "window creation error.";
    }
    else
    {
        connect(this, &App::serialTransportChanged, this, [=]() {
            if(_serial)
            {
                _view->setProperty("portNames", _serial->availablePorts());
                connect(_serial, &SerialTransport::msgReceived, this, [=](const char* msg)
                    {
                        _disp->handleMessage(QString::fromLocal8Bit(msg));
                    });
            }
            });
        connect(this, &App::messageDispatcherChanged, this, [=]() {
            if(_disp)
            connect(_disp, &MessageDispatcher::transferMessage, this, [=](const QString& msg)
                {
                    _serial->write(msg.toStdString().c_str());
                });
            connect(_disp, &MessageDispatcher::modeChanged, this, [=]()
                {
                    _view->setProperty("mode",_disp->mode());
                });
            connect(_disp, &MessageDispatcher::sensorChanged, this, [=]()
                {
                    _view->setProperty("sensor", _disp->sensor());
                });
            });
        setSerialTransport(new SerialTransport(this));
        setMessageDispatcher(new MessageDispatcher(this));

        connect(_view, SIGNAL(connectToPort(const QString&)),
            this, SLOT(handleConnect(const QString&)));
        connect(_view, SIGNAL(manualChanged()),
            this, SLOT(handleManualChange()));
        connect(_view, SIGNAL(modeChanged()),
            this, SLOT(handleModeChange()));
        qCDebug(LC_APP) << "created.";
    }
}
void App::handleConnect(const QString& port)
{
    _serial->openPort(port);
    _view->setProperty("pageState", "connecting");

    //waits until the timeoutTimer runs out or until mode and illum are received  
    QTimer timeoutTimer;
    timeoutTimer.setSingleShot(true);
    QEventLoop loop; bool modeReceived = false, illumReceived = false;
    connect(_disp, &MessageDispatcher::modeChanged, &loop, [&]()
        {modeReceived = true; if (illumReceived) loop.exit(); });
    connect(_disp, &MessageDispatcher::sensorChanged, &loop, [&]()
        {illumReceived = true; if (modeReceived) loop.exit(); });
    connect(&timeoutTimer, &QTimer::timeout, &loop, &QEventLoop::quit);
    _disp->requestSensor();
    _disp->requestMode();
    timeoutTimer.start(2000);
    loop.exec();

    if (modeReceived && illumReceived)
    {
        _view->setProperty("pageState", "connected");
    }
    else
    {
        _view->setProperty("pageState", "disconnected");
        //add timeout logic
    }
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
    _qApp.exit(code);
    qCDebug(LC_APP) << "closed.";
}
void App::setSerialTransport(SerialTransport* other)
{
    if (other == _serial)
        return;
    if (_serial)
        disconnect(_serial, nullptr, this, nullptr);
    _serial = other;
    emit serialTransportChanged();
}
App::~App()
{
    delete _view;
}
SerialTransport* App::serialTransport() const
{
    return _serial;
}
void App::setMessageDispatcher(MessageDispatcher* other)
{
    if (_disp == other)
        return;
    if(_disp)
        disconnect(_disp, nullptr, this, nullptr);
    _disp = other;
    emit messageDispatcherChanged();
}
MessageDispatcher * App::messageDispatcher() const
{
    return _disp;
}
void App::handleModeChange()
{
    _disp->setMode(_view->property("mode").toString());
}
void App::handleManualChange()
{
    _disp->setManual(_view->property("manual").toDouble());
}