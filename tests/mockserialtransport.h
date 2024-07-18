#include "serialtransport.h"
#include <gmock/gmock.h>
#include <qobject.h>
#include "qstringoutput.h"
class MockSerialTransport : public SerialTransport
{
    Q_OBJECT;
public:
    explicit MockSerialTransport(QObject* parent = nullptr) : SerialTransport(parent) {}
    MOCK_METHOD(QStringList, availablePorts, (), (const,override));
    MOCK_METHOD(QString, openedPort, (), (const, override));
    MOCK_METHOD(bool, openPort, (const QString&), (override));
    MOCK_METHOD(void, closePort,(),(override));
    MOCK_METHOD(int, write, (const char*), (override));
};