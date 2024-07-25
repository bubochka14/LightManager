#ifndef ATRANSPORT_H
#define ATRANSPORT_H
#include <QObject>
#include <qiodevice.h>
#include <QString>
#include <qregularexpression.h>
class MessageDispatcher: public QObject
{
    Q_OBJECT;
    Q_PROPERTY(QString mode READ mode WRITE setMode NOTIFY modeChanged);
    Q_PROPERTY(double sensor READ sensor NOTIFY sensorChanged);
    Q_PROPERTY(double manual READ manual WRITE setManual NOTIFY manualChanged);
public:
    explicit MessageDispatcher(QObject* parent = nullptr);
    virtual ~MessageDispatcher();
    virtual void setManual(double other);
    virtual double manual() const;
    virtual double sensor() const;
    virtual void setMode(const QString& other);
    virtual QString mode() const;
public slots:
    virtual void requestMode();
    virtual void requestSensor();
    virtual void handleMessage(const QString& message);
signals:
    void modeChanged();
    void sensorChanged();
    void manualChanged();
    void transferMessage(const QString&);
protected:
    void setSensor(double other);
private:
    QString _messageCache;
    QString _mode;
    double _sensor;
    double _manual;
};

#endif // ATRANSPORT_H
