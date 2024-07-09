#pragma once
#include <QObject>
class ATransport : public QObject
{
	Q_OBJECT;
public:
	virtual int write(const char* msg)=0;
signals:
	void msgReceived(const char* msg);
protected:
	explicit ATransport(QObject* parent = nullptr);
};