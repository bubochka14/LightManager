
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
};


TEST_F(AppFixture, UIPortConnectTest)
{
    ASSERT_NE(window, nullptr);
    QSignalSpy connectSpy(window, SIGNAL(connectToPort(const QString&)));
    auto portCombo = window->findChild<QQuickItem*>("portCombo");
    auto connectBtn = window->findChild<QQuickItem*>("connectBtn");
    auto serialMock = new MockSerialTransport(app);
    auto dispMock = new MockMessageDispatcher(app);
    ASSERT_NE(portCombo, nullptr);
    ASSERT_NE(connectBtn, nullptr);

    ON_CALL(*serialMock, availablePorts())
        .WillByDefault(Return(QStringList() << "test_port1" << "test_port2" << "test_port3"));
    EXPECT_CALL(*serialMock, openPort(QString("test_port2"))).Times(1);;

    app->setSerialTransport(serialMock);
    EXPECT_EQ(portCombo->property("count").toInt(), 3);
    int index;
    QMetaObject::invokeMethod(portCombo, "find",
        Q_RETURN_ARG(int, index),
        Q_ARG(QString, "test_port2")
    );
    ASSERT_NE(index, -1);
    portCombo->setProperty("currentIndex", index);
    QString beforeRequestState, afterRequestState;
    EXPECT_CALL(*dispMock, requestMode())
        .Times(1)
        .WillOnce(Invoke([&]() {
        beforeRequestState = window->property("pageState").toString();
            }));
    EXPECT_CALL(*dispMock, requestSensorIlluminance()).Times(1);
    mouseClick(connectBtn);
    ASSERT_EQ(connectSpy.count(), 1);
    EXPECT_EQ(connectSpy.at(0).at(0).toString(), "test_port2");
    EXPECT_EQ(beforeRequestState, "connecting");
    emit dispMock->modeChanged("manual");
    emit dispMock->sensorIlluminanceChanged(0.5);
    EXPECT_EQ(window->property("pageState").toString(), "connected");
}
TEST_F(AppFixture, UISensorIlluminanceChangingTest)
{
    ASSERT_NE(window, nullptr);
    auto mockDispatcher = new MockMessageDispatcher(app);
    auto dummySerial = new MockSerialTransport(app);
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
    auto dummySerial = new MockSerialTransport();
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
    EXPECT_TRUE(manualModeRadio->property("checked").toBool());
    EXPECT_FALSE(sensorModeRadio->property("checked").toBool());
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
    auto dummySerial = new MockSerialTransport(app);
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
