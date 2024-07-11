#include <gtest/gtest.h>
#include "app.h"
#include <QQuickWindow>
#include <QMetaObject>
#include <QAbstractItemModel>
#include <QTest>
#include <QTimer>
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
	App* app;
	QQuickWindow* window;
};
TEST(app_test, app_creation)
{
	ASSERT_NO_THROW(App(_argc,_argv));
}
int main(int argc, char** argv) {
	_argc = argc; _argv = argv;
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}