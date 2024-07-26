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
TEST_F(MessageDispatcherFixture, HandleCorrectMessage)
{
	QSignalSpy modeSpy(disp, SIGNAL(modeChanged()));
	QSignalSpy sensorSpy(disp, SIGNAL(sensorChanged()));
	disp->handleMessage("mode:test_mode1\n");
	EXPECT_EQ(disp->mode(), "test_mode1");
	disp->handleMessage("mode:test_mode2\n");
	EXPECT_EQ(disp->mode(), "test_mode2");
	disp->handleMessage("sensor:0.12345\n");
	EXPECT_EQ(disp->sensor(), 0.12);

	ASSERT_EQ(sensorSpy.count(), 1);
	ASSERT_EQ(modeSpy.count(), 2);
}
TEST_F(MessageDispatcherFixture, RoundingSesnor)
{
	QSignalSpy sensorSpy(disp, SIGNAL(sensorChanged()));
	disp->handleMessage("sensor:0\n");
	EXPECT_EQ(disp->sensor(), 0);
	disp->handleMessage("sensor:0.495\n");
	EXPECT_EQ(disp->sensor(), 0.5);
	disp->handleMessage("sensor:0.455\n");
	EXPECT_EQ(disp->sensor(), 0.46);
	disp->handleMessage("sensor:12\n");
	EXPECT_EQ(disp->sensor(),1);
	disp->handleMessage("sensor:0.999\n");
	EXPECT_EQ(disp->sensor(), 1);
	ASSERT_EQ(sensorSpy.count(), 4);

}
TEST_F(MessageDispatcherFixture, RoundingManual)
{
	QSignalSpy transferSpy(disp, SIGNAL(transferMessage(const QString&)));
	disp->setManual(0.455);
	disp->setManual(0);
	disp->setManual(0.495);
	disp->setManual(12);
	disp->setManual(0.999);

	ASSERT_EQ(transferSpy.count(), 5);
	EXPECT_EQ(transferSpy.takeFirst().first().toString(), "manual:0.46\n");
	EXPECT_EQ(transferSpy.takeFirst().first().toString(), "manual:0\n");
	EXPECT_EQ(transferSpy.takeFirst().first().toString(), "manual:0.5\n");
	EXPECT_EQ(transferSpy.takeFirst().first().toString(), "manual:1\n");
	EXPECT_EQ(transferSpy.takeFirst().first().toString(), "manual:1\n");
}
TEST_F(MessageDispatcherFixture, HandleMessagesInPieces)
{
	QSignalSpy modeSpy(disp, SIGNAL(modeChanged()));
	QSignalSpy sensorSpy(disp, SIGNAL(sensorChanged()));
	disp->handleMessage("sensor:");
	disp->handleMessage("0.123\n");
	EXPECT_EQ(disp->sensor(), 0.12);
	disp->handleMessage("mo");
	disp->handleMessage("d");
	disp->handleMessage("e:manual\n");
	EXPECT_EQ(disp->mode(), "manual");
	disp->handleMessage("sensor:0.5\nmode:sensor\ns");
	EXPECT_EQ(disp->mode(), "sensor");
	EXPECT_EQ(disp->sensor(), 0.5);
	disp->handleMessage("ensor:0.1\n");
	EXPECT_EQ(disp->sensor(), 0.1);

	ASSERT_EQ(sensorSpy.count(), 3);
	ASSERT_EQ(modeSpy.count(),2);
}
TEST_F(MessageDispatcherFixture, IncorrectPostMessage)
{
	QSignalSpy modeSpy(disp, SIGNAL(modeChanged(const QString&)));
	QSignalSpy sensorSpy(disp, SIGNAL(sensorIlluminanceChanged(double)));
	disp->handleMessage("mde:test_mode1\n");
	disp->handleMessage("mode:\n");
	disp->handleMessage("mode\n");
	disp->handleMessage("sensor:abc\n");
	disp->handleMessage("sen\nsor:12\n");
	disp->handleMessage(":\n");
	disp->handleMessage("2\n");
	disp->handleMessage("senso");
	disp->handleMessage("r:0.12345");

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