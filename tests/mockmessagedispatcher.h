#pragma once
#include <gmock/gmock.h>
#include "messagedispatcher.h"
#include "qstringoutput.h"

class MockMessageDispatcher : public MessageDispatcher
{
	Q_OBJECT;
public:
	explicit MockMessageDispatcher(QObject* parent = nullptr) : MessageDispatcher(parent) {}
	MOCK_METHOD(void, postMessage,(const QString&),(override));
	MOCK_METHOD(void, setManualIllumination,(double),(override));
	MOCK_METHOD(void, setMode,(const QString&),(override));
	MOCK_METHOD(void, requestMode,(),(override));
	MOCK_METHOD(void, requestSensorIlluminance,(),(override));
};