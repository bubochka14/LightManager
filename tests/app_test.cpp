#include <gtest/gtest.h>
#include "app.h"
#include <QQuickWindow>
#include <QMetaObject>
#include <QAbstractItemModel>
#include <QTest>
#include <QTimer>
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
		exitTimer.start(100);
		app->exec();
		delete app;
	}
    void mouseClick(QPoint pos, Qt::MouseButton btn = Qt::LeftButton)
    {
        QTest::mouseClick(window, btn, Qt::KeyboardModifiers(), pos);
    }
	App* app;
	QQuickWindow* window;
};
TEST(app_test, app_creation)
{
	ASSERT_NO_THROW(App(_argc,_argv));
}
TEST_F(AppFixture, WindowCreation)
{
    ASSERT_NE(window, nullptr);
}
TEST_F(AppFixture, UiPortsTest)
{
    ASSERT_NE(window, nullptr);

    auto portCombo = window->findChild<QQuickItem*>("portCombo");
    auto serialMock1 = std::make_shared<NiceMock<MockSerialTransport>>();
    auto serialMock2 = std::make_shared<NiceMock<MockSerialTransport>>();
    EXPECT_CALL(*serialMock2, availablePorts()).Times(AtLeast(1)).
        WillRepeatedly(Return(QStringList() << "test_port3" << "test_port4" << "test_port5"));
    EXPECT_CALL(*serialMock1, availablePorts()).Times(AtLeast(1)).
        WillRepeatedly(Return(QStringList() << "test_port1" << "test_port2"));
    ASSERT_NE(portCombo, nullptr);

    app->setSerialTransport(serialMock1);
    EXPECT_EQ(portCombo->property("count").toInt(), 2);
    int index;
    QMetaObject::invokeMethod(portCombo, "find",
        Q_RETURN_ARG(int, index),
        Q_ARG(QString, "testPort2")
    );
    ASSERT_NE(index, -1);

    app->setSerialTransport(serialMock2);
    EXPECT_EQ(portCombo->property("count").toInt(), 3);
    QMetaObject::invokeMethod(portCombo, "find",
        Q_RETURN_ARG(int, index),
        Q_ARG(QString, "testPort5")
    );
    ASSERT_NE(index, -1);
}
TEST_F(AppFixture, UISensorIlluminanceChangingTest)
{
    ASSERT_NE(window, nullptr);
    auto mockDispatcher = std::make_shared<MockMessageDispatcher>();
    auto dummySerial = std::make_shared<MockSerialTransport>();
    app->setMessageDispatcher(mockDispatcher);
    app->setSerialTransport(dummySerial);
    auto icon = window->findChild<QQuickItem*>("sensorIcon");
    auto lbl = window->findChild<QQuickItem*>("sensorIlluminance");

    ASSERT_NE(icon, nullptr);
    ASSERT_NE(lbl, nullptr);

    mockDispatcher->sensorIlluminanceChanged(0.556);
    EXPECT_TRUE(lbl->property("text").toString().contains(QRegularExpression("56$")));
    EXPECT_EQ(icon->property("state").toString(), QString("day"));
    mockDispatcher->sensorIlluminanceChanged(0.111);
    EXPECT_EQ(icon->property("state").toString(), QString("night"));
    EXPECT_TRUE(lbl->property("text").toString().contains(QRegularExpression("11$")));
}
TEST_F(AppFixture, UISettingWorkmodeTest)
{
    ASSERT_NE(window, nullptr);
    auto mockDispatcher = std::make_shared<MockMessageDispatcher>();
    auto dummySerial = std::make_shared<MockSerialTransport>();
    app->setMessageDispatcher(mockDispatcher);
    app->setSerialTransport(dummySerial);
    auto sensorModeRadio = window->findChild<QQuickItem*>("sensorModeRadio");
    auto manualModeRadio = window->findChild<QQuickItem*>("manualModeRadio");

    ASSERT_NE(sensorModeRadio, nullptr);
    ASSERT_NE(manualModeRadio, nullptr);

    mockDispatcher->modeChanged("manual");
    EXPECT_TRUE(manualModeRadio->property("checked").toBool());
    EXPECT_FALSE(sensorModeRadio->property("checked").toBool());
    mockDispatcher->modeChanged("sensor");
    EXPECT_TRUE(manualModeRadio->property("checked").toBool());
    EXPECT_FALSE(sensorModeRadio->property("checked").toBool());
    EXPECT_CALL(*mockDispatcher, setMode(QString("sensor")))
        .Times(1);
    EXPECT_CALL(*mockDispatcher, setMode(QString("manual")))
        .Times(1);
    mouseClick(manualModeRadio->position().toPoint());
    mouseClick(sensorModeRadio->position().toPoint());

}
TEST_F(AppFixture, UIManualSettingTest)
{
    ASSERT_NE(window, nullptr);
    auto mockDispatcher = std::make_shared<MockMessageDispatcher>();
    auto dummySerial = std::make_shared<MockSerialTransport>();
    app->setMessageDispatcher(mockDispatcher);
    app->setSerialTransport(dummySerial);
    auto dial = window->findChild<QQuickItem*>("manualModeDial");
    auto radio = window->findChild<QQuickItem*>("manualModeRadio");
    auto lbl = window->findChild<QQuickItem*>("manualModeLbl");

    ASSERT_NE(dial, nullptr);
    ASSERT_NE(radio, nullptr);
    ASSERT_NE(lbl, nullptr);
    EXPECT_CALL(*mockDispatcher, setManualIllumination(0.57))
        .Times(1);
    EXPECT_CALL(*mockDispatcher, setManualIllumination(0.12))
        .Times(1);
    mouseClick(radio->position().toPoint());
    dial->setProperty("value", 0.123);
    EXPECT_EQ(lbl->property("text").toString().contains(QRegularExpression("12$")), true);
    dial->setProperty("value", 0.565);
    EXPECT_EQ(lbl->property("text").toString().contains(QRegularExpression("57$")), true);

}
int main(int argc, char** argv) {
	_argc = argc; _argv = argv;
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}