#include <gtest/gtest.h>
#include <messagedispatcher.h>
#include <qsignalspy.h>
#include <thread>
#include <chrono>

using namespace Qt::Literals::StringLiterals;
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
TEST_F(MessageDispatcherFixture, CorrectPostMessageTest)
{
	QSignalSpy modeSpy(disp, SIGNAL(modeChanged(const QString&)));
	QSignalSpy sensorSpy(disp, SIGNAL(sensorValueChanged(int)));
	disp->postMessage("mode:test_mode1\n"); 
	disp->postMessage("mode:test_mode"); 
	disp->postMessage("2\n"); 
	disp->postMessage("senso");
	disp->postMessage("r:0.12345\n");

	ASSERT_EQ(sensorSpy.count(), 2);
	ASSERT_EQ(modeSpy.count(), 2);

	QList<QVariant> args = sensorSpy.takeFirst();
	EXPECT_EQ(args.at(0).toDouble(), 0.12345);
	args = sensorSpy.takeFirst();
	EXPECT_EQ(args.at(0).toDouble(), 1.9);

	args = modeSpy.takeFirst();
	EXPECT_EQ(args.at(0).toString(), "test_mode1)"_L1);
	args = modeSpy.takeFirst();
	EXPECT_EQ(args.at(0).toString(), "test_mode2"_L1);

}
TEST_F(MessageDispatcherFixture, IncorrectPostTest)
{
	QSignalSpy modeSpy(disp, SIGNAL(modeChanged(const QString&)));
	QSignalSpy sensorSpy(disp, SIGNAL(sensorValueChanged(int)));
	disp->postMessage("mde:test_mode1\n");
	disp->postMessage("mode:\n");
	disp->postMessage("mode\n");
	disp->postMessage("sensor:abc\n");
	disp->postMessage("sen\nsor:12\n");
	disp->postMessage(":\n");
	std::this_thread::sleep_for(std::chrono::seconds(2));
	disp->postMessage("2\n");
	disp->postMessage("senso");
	std::this_thread::sleep_for(std::chrono::seconds(2));
	disp->postMessage("r:0.12345\n");

	EXPECT_EQ(sensorSpy.count(), 0);
	EXPECT_EQ(modeSpy.count(), 0);

}
TEST_F(MessageDispatcherFixture, TransferMessageTest)
{
	QSignalSpy transferSpy(disp, SIGNAL(transferMessage(const char*)));
	disp->setMode("test_mode1");
	disp->setMode("test_mode2");
	disp->setUserIllumination(1.2);
	disp->setUserIllumination(12);


	ASSERT_EQ(transferSpy.count(),4);
	QList<QVariant> args = transferSpy.takeFirst();
	EXPECT_EQ(args.at(0).toString(), "mode:test_mode1\n"_L1);
	args = transferSpy.takeFirst();
	EXPECT_EQ(args.at(0).toString(), "mode:test_mode2\n"_L1);
	args = transferSpy.takeFirst();
	EXPECT_EQ(args.at(0).toString(),"user:1.2\n"_L1);
	args = transferSpy.takeFirst();
	EXPECT_EQ(args.at(0).toString(), "user:12\n"_L1);
}