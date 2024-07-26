#pragma once
#include "transportfactory.h"
#include <qqmlengine.h>
class SerialTransportFactory : public TransportFactory
{
	Q_OBJECT;
	Q_PROPERTY(QString port READ port WRITE setPort NOTIFY portChanged REQUIRED)
public:
	explicit SerialTransportFactory(QObject* parent = nullptr);
	QIODevice* createTransport() override;
	void setPort(const QString& port);
	QString port() const;
signals:
	void portChanged();
private:
	QString _port;
};