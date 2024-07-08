#ifndef APP_H
#define APP_H
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlComponent>
#include <QQuickItem>
#include <QSettings>
class App
{
public:
    explicit App(int argc, char **argv);
    int exec();
    void exit(int code=0);
public:
    QQmlApplicationEngine * engine() const;
    ~App();
private:
    std::shared_ptr<QGuiApplication> _qApp;
    std::shared_ptr<QQmlApplicationEngine> _engine;

    QStringList _ports;
    QObject * _view;
    QSettings _sett;
};

#endif // APP_H
