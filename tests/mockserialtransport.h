#include "serialtransport.h"
#include <gmock/gmock.h>
class MockSerialTransport : public SerialTransport
{
public:
    MOCK_METHOD(QStringList, availablePorts, (), (const,override));
    MOCK_METHOD(QString, openedPort, (), (const, override));
    MOCK_METHOD(bool, openPort, (const QString&), (override));
    MOCK_METHOD(void, closePort,(),(override));
    MOCK_METHOD(int, write, (const char*), (override));
};