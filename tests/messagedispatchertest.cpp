#include <gtest/gtest.h>
#include <messagedispatcher.h>
#include <qsignalspy.h>
#include <thread>
#include <chrono>
#include "qstringoutput.h"

using namespace testing;

class MessageDispatcherFixture : public  Test
{
protected:
	MessageDispatcherFixture() = default;
	void SetUp() override
	{
		disp = new MessageDispatcher;
	}
	void TearDown() override
	{
		delete disp;
	}
	MessageDispatcher* disp;
};
TEST_F(MessageDispatcherFixture, CorrectPostMessage)
{
	QSignalSpy modeSpy(disp, SIGNAL(modeChanged(const QString&)));
	QSignalSpy sensorSpy(disp, SIGNAL(sensorIlluminanceChanged(double)));
	disp->postMessage("mode:test_mode1\n"); 
	disp->postMessage("mode:test_mode2\n"); 
	disp->postMessage("sensor:0.12345\n");

	ASSERT_EQ(sensorSpy.count(), 1);
	ASSERT_EQ(modeSpy.count(), 2);
	EXPECT_EQ(sensorSpy.takeFirst().at(0).toDouble(), 0.12);
	EXPECT_STREQ(modeSpy.takeFirst().at(0).toString().toStdString().c_str(), "test_mode1");
	EXPECT_STREQ(modeSpy.takeFirst().at(0).toString().toStdString().c_str(), "test_mode2");

}
TEST_F(MessageDispatcherFixture, RoundingSesnor)
{
	QSignalSpy sensorSpy(disp, SIGNAL(sensorIlluminanceChanged(double)));
	disp->postMessage("sensor:0.455\n");
	disp->postMessage("sensor:0\n");
	disp->postMessage("sensor:0.495\n");
	disp->postMessage("sensor:12\n");
	disp->postMessage("sensor:0.999\n");

	ASSERT_EQ(sensorSpy.count(), 5);
	EXPECT_EQ(sensorSpy.takeFirst().at(0).toDouble(), 0.46);
	EXPECT_EQ(sensorSpy.takeFirst().at(0).toDouble(), 0);
	EXPECT_EQ(sensorSpy.takeFirst().at(0).toDouble(), 0.5);
	EXPECT_EQ(sensorSpy.takeFirst().at(0).toDouble(),1);
	EXPECT_EQ(sensorSpy.takeFirst().at(0).toDouble(), 1);
}
TEST_F(MessageDispatcherFixture, RoundingManual)
{
	QSignalSpy transferSpy(disp, SIGNAL(transferMessage(const QString&)));
	disp->setManualIllumination(0.455);
	disp->setManualIllumination(0);
	disp->setManualIllumination(0.495);
	disp->setManualIllumination(12);
	disp->setManualIllumination(0.999);

	ASSERT_EQ(transferSpy.count(), 5);
	EXPECT_STREQ(transferSpy.takeFirst().first().toString().toStdString().c_str(), "manual:0.46\n");
	EXPECT_STREQ(transferSpy.takeFirst().first().toString().toStdString().c_str(), "manual:0\n");
	EXPECT_STREQ(transferSpy.takeFirst().first().toString().toStdString().c_str(), "manual:0.5\n");
	EXPECT_STREQ(transferSpy.takeFirst().first().toString().toStdString().c_str(), "manual:1\n");
	EXPECT_STREQ(transferSpy.takeFirst().first().toString().toStdString().c_str(), "manual:1\n");
}
TEST_F(MessageDispatcherFixture, PostMessagesInPieces)
{
	QSignalSpy modeSpy(disp, SIGNAL(modeChanged(const QString&)));
	QSignalSpy sensorSpy(disp, SIGNAL(sensorIlluminanceChanged(double)));
	disp->postMessage("sensor:");
	disp->postMessage("0.123\n");
	disp->postMessage("mo");
	disp->postMessage("d");
	disp->postMessage("e:manual\n");
	disp->postMessage("sensor:0.5\nmode:sensor\ns");
	disp->postMessage("ensor:0.1\n");

	ASSERT_EQ(sensorSpy.count(), 3);
	ASSERT_EQ(modeSpy.count(),2);
	EXPECT_EQ(sensorSpy.takeFirst().first().toDouble(), 0.12);
	EXPECT_EQ(sensorSpy.takeFirst().first().toDouble(), 0.5);
	EXPECT_EQ(sensorSpy.takeFirst().first().toDouble(), 0.1);
	EXPECT_STREQ(modeSpy.takeFirst().first().toString().toStdString().c_str(), "manual");
	EXPECT_STREQ(modeSpy.takeFirst().first().toString().toStdString().c_str(), "sensor");
}
TEST_F(MessageDispatcherFixture, IncorrectPostMessage)
{
	QSignalSpy modeSpy(disp, SIGNAL(modeChanged(const QString&)));
	QSignalSpy sensorSpy(disp, SIGNAL(sensorIlluminanceChanged(double)));
	disp->postMessage("mde:test_mode1\n");
	disp->postMessage("mode:\n");
	disp->postMessage("mode\n");
	disp->postMessage("sensor:abc\n");
	disp->postMessage("sen\nsor:12\n");
	disp->postMessage(":\n");
	disp->postMessage("2\n");
	disp->postMessage("senso");
	disp->postMessage("r:0.12345");

	EXPECT_EQ(sensorSpy.count(), 0);
	EXPECT_EQ(modeSpy.count(), 0);

}
TEST_F(MessageDispatcherFixture, SetMode)
{
	QSignalSpy transferSpy(disp, SIGNAL(transferMessage(const QString&)));
	disp->setMode("test_mode1");
	disp->setMode("test_mode2");

	ASSERT_EQ(transferSpy.count(),2);
	EXPECT_STREQ(transferSpy.takeFirst().first().toString().toStdString().c_str(), "mode:test_mode1\n");
	EXPECT_STREQ(transferSpy.takeFirst().first().toString().toStdString().c_str(), "mode:test_mode2\n");
}