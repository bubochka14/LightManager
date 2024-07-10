#include <gmock/gmock.h>
#include "messagedispatcher.h"
class MockMessageDispatcher : public MessageDispatcher
{
public:
	MOCK_METHOD(bool, postMessage,(const char*),(override));
	MOCK_METHOD(bool, setUserIllumination,(double),(override));
	MOCK_METHOD(bool, setMode,(const QString&),(override));
	
};