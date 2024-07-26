#pragma once
#include <gmock/gmock.h>
#include "messagedispatcher.h"
#include "qstringoutput.h"

class MockMessageDispatcher : public MessageDispatcher
{
	Q_OBJECT;
public:
	explicit MockMessageDispatcher(QObject* parent = nullptr) : MessageDispatcher(parent) {}
	MOCK_METHOD(double, manual,(),(const,override));
	MOCK_METHOD(double, sensor,(),(const,override));
	MOCK_METHOD(void, setManual,(double),(override));
	MOCK_METHOD(void, setMode,(const QString&),(override));
	MOCK_METHOD(QString, mode,(),(const,override));
	MOCK_METHOD(void, requestMode,(),(override));
	MOCK_METHOD(void, requestSensor,(),(override));
	MOCK_METHOD(void, handleMessage,(const QString&),(override));
};