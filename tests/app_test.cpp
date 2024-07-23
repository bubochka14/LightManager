
#include <gtest/gtest.h>
#include "app.h"
#include <QQuickWindow>
#include <QMetaObject>
#include <QAbstractItemModel>
#include <QTest>
#include <QTimer>
#include <QSignalSpy>
#include "mockserialtransport.h"
#include "mockmessagedispatcher.h"
#include "qstringoutput.h"

using namespace testing;
int _argc;
char** _argv;
class AppFixture : public Test
{
protected:
    void SetUp() override
    {
        //char** dupArgv;
        //for (size_t i = 0; i < _argc; i++)
        //{
        //    dupArgv[i] = strdup(_argv[i]);
        //}
        app = new App(_argc, _argv);
        window = app->engine()->rootObjects().isEmpty() ? nullptr
            : (QQuickWindow*)app->engine()->rootObjects().first();
    }
    void TearDown() override
    {
        QTimer exitTimer;
        exitTimer.setSingleShot(true);
        QObject::connect(&exitTimer, &QTimer::timeout, [=]() {app->exit(0); });
        exitTimer.start(500);
        app->exec();
        delete app;
    }
    void mouseClick(QPoint pos, Qt::MouseButton btn = Qt::LeftButton)
    {
        QTest::mouseClick(window, btn, Qt::KeyboardModifiers(), pos);
    }
    void mouseClick(QQuickItem* item, Qt::MouseButton btn = Qt::LeftButton)
    {
        QTest::mouseClick(window, btn, Qt::KeyboardModifiers(), 
            QPoint(item->x() + item->width() / 2, item->y() + item->height()/2));
    }
    App* app;
    QQuickWindow* window;
    char** dupArgv;
};


TEST_F(AppFixture, SerialAndDispatcherConnect)
{
    ASSERT_NE(window, nullptr);
    auto mockDispatcher = new NiceMock < MockMessageDispatcher>(app);
    auto mockSerial = new NiceMock<MockSerialTransport>(app);
    app->setMessageDispatcher(mockDispatcher);
    app->setSerialTransport(mockSerial);
    EXPECT_CALL(*mockSerial, write(_)).Times(2);
    EXPECT_CALL(*mockDispatcher, postMessage(_)).Times(2);
    mockDispatcher->transferMessage("1");
    mockDispatcher->transferMessage("2");
    mockSerial->msgReceived("mode:test");
    mockSerial->msgReceived("sensor:3");
}
TEST_F(AppFixture, UIPortsDisplay)
{
    ASSERT_NE(window, nullptr);
    auto portCombo = window->findChild<QQuickItem*>("portCombo");
    ASSERT_NE(portCombo, nullptr);
    auto serialMock1 = new NiceMock<MockSerialTransport>(app);
    auto serialMock2 = new NiceMock<MockSerialTransport>(app);
    EXPECT_CALL(*serialMock1, availablePorts()).Times(AtLeast(1))
        .WillOnce(Return(QStringList() << "test_port1" << "test_port2" << "test_port3"));
    EXPECT_CALL(*serialMock2, availablePorts()).Times(AtLeast(1))
        .WillOnce(Return(QStringList()));
    app->setSerialTransport(serialMock1);
    EXPECT_EQ(portCombo->property("count").toInt(), 3);
    int index;
    QMetaObject::invokeMethod(portCombo, "find",
        Q_RETURN_ARG(int, index),
        Q_ARG(QString, "test_port2")
    );
    ASSERT_NE(index, -1);
    app->setSerialTransport(serialMock2);
    EXPECT_EQ(portCombo->property("count").toInt(), 0);
    EXPECT_FALSE(portCombo->property("enabled").toBool());

}
TEST_F(AppFixture, UIConnectWithResponse)
{
    ASSERT_NE(window, nullptr);
    QSignalSpy connectSpy(window, SIGNAL(connectToPort(const QString&)));
    auto portCombo = window->findChild<QQuickItem*>("portCombo");
    auto connectBtn = window->findChild<QQuickItem*>("connectBtn");
    auto serialMock = new NiceMock<MockSerialTransport>(app);
    auto dispMock = new StrictMock<MockMessageDispatcher>(app);
    ASSERT_NE(portCombo, nullptr);
    ASSERT_NE(connectBtn, nullptr);

    ON_CALL(*serialMock, availablePorts())
        .WillByDefault(Return(QStringList() << "test_port1"));
    EXPECT_CALL(*serialMock, openPort(QString("test_port1"))).Times(1);;
    app->setMessageDispatcher(dispMock);
    app->setSerialTransport(serialMock);
    QString beforeRequestState, afterRequestState;
    EXPECT_CALL(*dispMock, requestMode())
        .Times(1)
        .WillOnce(Invoke([&]() {
            emit dispMock->modeChanged("manual");
            beforeRequestState = window->property("pageState").toString();
            }));
    EXPECT_CALL(*dispMock, requestSensorIlluminance())
        .Times(1)
        .WillOnce(Invoke([&]() {
            emit dispMock->sensorIlluminanceChanged(0.5);
            }));
    mouseClick(connectBtn);
    ASSERT_EQ(connectSpy.count(), 1);
    EXPECT_EQ(connectSpy.at(0).at(0).toString(), "test_port1");
    EXPECT_EQ(beforeRequestState, "connecting");
    EXPECT_EQ(window->property("pageState").toString(), "connected");
}
TEST_F(AppFixture, UIConnectWithNoResponse)
{
    ASSERT_NE(window, nullptr);
    auto portCombo = window->findChild<QQuickItem*>("portCombo");
    auto connectBtn = window->findChild<QQuickItem*>("connectBtn");
    auto serialMock = new NiceMock<MockSerialTransport>(app);
    auto dispMock = new NiceMock<MockMessageDispatcher>(app);
    ASSERT_NE(portCombo, nullptr);
    ASSERT_NE(connectBtn, nullptr);

    ON_CALL(*serialMock, availablePorts())
        .WillByDefault(Return(QStringList() << "test_port1"));
    app->setMessageDispatcher(dispMock);
    app->setSerialTransport(serialMock);
    mouseClick(connectBtn);
    std::this_thread::sleep_for(std::chrono::microseconds(app->connectionTimeout()));
    EXPECT_EQ(window->property("pageState").toString(), "disconnected");
}
TEST_F(AppFixture, UISensorIlluminanceChanging)
{
    ASSERT_NE(window, nullptr);
    auto mockDispatcher = new MockMessageDispatcher(app);
    auto dummySerial = new NiceMock<MockSerialTransport>(app);
    app->setMessageDispatcher(mockDispatcher);
    app->setSerialTransport(dummySerial);
    auto icon = window->findChild<QQuickItem*>("sensorIcon");
    auto lbl = window->findChild<QQuickItem*>("sensorIlluminance");
    ASSERT_NE(icon, nullptr);
    ASSERT_NE(lbl, nullptr);

    mockDispatcher->sensorIlluminanceChanged(0.56);
    EXPECT_TRUE(lbl->property("text").toString().contains(QRegularExpression("56$"))) 
        << " text in lbl: " << lbl->property("text").toString().toStdString();
    EXPECT_STREQ(icon->property("state").toString().toStdString().c_str(), "day");
    mockDispatcher->sensorIlluminanceChanged(0.11);
    EXPECT_STREQ(icon->property("state").toString().toStdString().c_str(), "night");
    EXPECT_TRUE(lbl->property("text").toString().contains(QRegularExpression("11$")))
        << " text in lbl: " << lbl->property("text").toString().toStdString();
}
TEST_F(AppFixture, UISettingWorkmodeTest)
{
    ASSERT_NE(window, nullptr);
    auto mockDispatcher = new MockMessageDispatcher(app);
    auto dummySerial = new NiceMock<MockSerialTransport>();
    app->setMessageDispatcher(mockDispatcher);
    app->setSerialTransport(dummySerial);
    auto sensorModeRadio = window->findChild<QQuickItem*>("sensorModeRadio");
    auto manualModeRadio = window->findChild<QQuickItem*>("manualModeRadio");

    ASSERT_NE(sensorModeRadio, nullptr);
    ASSERT_NE(manualModeRadio, nullptr);
    window->setProperty("pageState", "connected");
    emit mockDispatcher->modeChanged("manual");
    EXPECT_TRUE(manualModeRadio->property("checked").toBool());
    EXPECT_FALSE(sensorModeRadio->property("checked").toBool());
    emit mockDispatcher->modeChanged("sensor");
    EXPECT_FALSE(manualModeRadio->property("checked").toBool());
    EXPECT_TRUE(sensorModeRadio->property("checked").toBool());
    EXPECT_CALL(*mockDispatcher, setMode(QString("sensor")))
        .Times(1);
    EXPECT_CALL(*mockDispatcher, setMode(QString("manual")))
        .Times(1);
    mouseClick(manualModeRadio);
    mouseClick(sensorModeRadio);

}
TEST_F(AppFixture, UIManualSettingTest)
{
    ASSERT_NE(window, nullptr);
    auto mockDispatcher = new MockMessageDispatcher(app);
    auto dummySerial = new NiceMock <MockSerialTransport>(app);
    app->setMessageDispatcher(mockDispatcher);
    app->setSerialTransport(dummySerial);
    auto dial = window->findChild<QQuickItem*>("manualModeDial");
    auto radio = window->findChild<QQuickItem*>("manualModeRadio");
    auto lbl = window->findChild<QQuickItem*>("manualIllumination");

    ASSERT_NE(dial, nullptr);
    ASSERT_NE(radio, nullptr);
    ASSERT_NE(lbl, nullptr);
    EXPECT_CALL(*mockDispatcher, setManualIllumination(0.56))
        .Times(1);
    EXPECT_CALL(*mockDispatcher, setManualIllumination(0.12))
        .Times(1);
    mouseClick(radio->position().toPoint());
    dial->setProperty("value", 0.12);
    EXPECT_TRUE(lbl->property("text").toString().contains(QRegularExpression("12$")));
    dial->setProperty("value", 0.56);
    EXPECT_TRUE(lbl->property("text").toString().contains(QRegularExpression("56$")));

}
int main(int argc, char** argv) {
    qDebug() << "Running tests with args: ";
    for (size_t i = 1; i < argc; i++)
    {
        qDebug() << QString::fromLatin1(argv[i]);
    }
    testing::InitGoogleTest(&argc, argv);
    _argc = argc; _argv = argv;
    return RUN_ALL_TESTS();
}
